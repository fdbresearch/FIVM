//===----------------------------------------------------------------------===//
//
// Copyright (c) 2010-2017 EPFL DATA Lab (http://data.epfl.ch)
// 
// Modified by FDB Research Group, University of Oxford
//
// https://fdbresearch.github.io/
//
//===----------------------------------------------------------------------===//
package fdbresearch.core

// import fdbresearch.core._
import fdbresearch.util.TypeHelper

/**
  * Fixes the M3 parse result to provide a correct AST and annotates 
  * the AST with appropriate types needed for code generation.
  *
  * @author Thierry Coppey, Milos Nikolic
  */
object TypeCheck extends (M3.System => M3.System) {

  import fdbresearch.util.Utils.{ fresh, freshClear, delta }
  import M3._

  def err(msg: String) = sys.error("Type checking error: " + msg)

  def getTargetMaps(s0: Statement): List[MapRef] = s0 match {
    case s: TriggerStmt => List(s.target)
    case s: IfStmt => s.thenBlk.flatMap(getTargetMaps) ++ s.elseBlk.flatMap(getTargetMaps)
  }

  // 1. Add used tables and batch updates as maps
  def addMissingMaps(s0: System) = {
    val accessedMaps = scala.collection.mutable.Set[String]()

    // Replace access by MapRefs (M3 fix?)
    def replaceExpr(e: Expr): Expr = e.replace {
      case m @ MapRefConst(n, ks, loc) =>
        accessedMaps += n
        MapRef(n, m.tp, ks)
      case m @ DeltaMapRefConst(n, ks, loc) =>
        accessedMaps += delta(n)
        MapRef(delta(n), m.tp, ks)
    }
    
    def replaceStmt(s0: Statement): Statement = s0 match {
      case s: TriggerStmt => 
        TriggerStmt(s.target, replaceExpr(s.expr), s.op, s.initExpr.map(replaceExpr))
      case s: IfStmt =>
        IfStmt(Cmp(replaceExpr(s.cond.l), replaceExpr(s.cond.r), s.cond.op),
          s.thenBlk.map(replaceStmt), s.elseBlk.map(replaceStmt))
    }

    def toLowerCase(fields: List[(String, Type)]) = 
      fields.map { case (n, t) => (n.toLowerCase, t) }

    def localityToLowerCase(l: LocalityType) = l match {
      case DistByKeyExp(ks) => DistByKeyExp(toLowerCase(ks))
      case _ => l
    }

    val queries = s0.queries.map { q => Query(q.name, replaceExpr(q.expr)) }
    
    val triggers = s0.triggers.map { t => Trigger(t.event, t.stmts.map(replaceStmt)) }

    val usedTableDefs = s0.sources.filter { s => 
        !s.isStream && accessedMaps.contains(s.schema.name)
      }.map { s => 
        MapDef(s.schema.name, TypeLong, toLowerCase(s.schema.fields),
          Const(TypeLong, "0"), localityToLowerCase(s.locality))
      } 
    val usedDeltaDefs = 
      s0.sources.filter { s =>
        s.isStream && accessedMaps.contains(delta(s.schema.name))
      }.map { s =>
        MapDef(delta(s.schema.name), TypeLong, toLowerCase(s.schema.fields),
          Const(TypeLong, "0"), localityToLowerCase(s.locality))
      }

    System(s0.typeDefs, s0.sources, s0.maps ::: usedTableDefs ::: usedDeltaDefs, queries, triggers)
  }

  // 2. Prettify variable names (not streams, not maps) 
  //    using a renaming function
  // 3. Rename M3 functions into implementation functions: 
  //    converting to lower case
  def renameVarsAndFuns (vn: String => String, fn: String => String) (s0: System) = {
    val globalMaps = s0.maps.map(_.name).toSet
    var localMapRenaming: Map[String, String] = null

    def renameKeys(keys: List[(String, Type)]) = 
      keys.map { case (n, t) => (vn(n), t) }

    def renameSchema(s: Schema) = 
      Schema(s.name, renameKeys(s.fields))

    def renameExpr(e: Expr): Expr = e.replace {
      case Ref(n, tp) => 
        Ref(vn(n), tp)
      case MapRef(n, tp, ks, tmp, loc) => 
        MapRef(localMapRenaming.getOrElse(n, n), tp, renameKeys(ks), tmp)
      case m @ DeltaMapRefConst(n, ks, loc) =>
        MapRef(delta(n), m.tp, renameKeys(ks))
      case Lift(n, e) => 
        Lift(vn(n), renameExpr(e))
      case AggSum(ks, e) => 
        AggSum(renameKeys(ks), renameExpr(e))
      case Repartition(ks, e) => 
        Repartition(renameKeys(ks), renameExpr(e))
      case Apply(f, tp, args, targs) =>  
        Apply(fn(f), tp, args.map(renameExpr), targs)
    }

    def renameStmt(s0: Statement): Statement = s0 match {
      case s: TriggerStmt =>
        TriggerStmt(
          renameExpr(s.target).asInstanceOf[MapRef],
          renameExpr(s.expr),
          s.op,
          s.initExpr.map(renameExpr)
        )
      case s: IfStmt =>
        IfStmt(
          Cmp(renameExpr(s.cond.l), renameExpr(s.cond.r), s.cond.op),
          s.thenBlk.map(renameStmt),
          s.elseBlk.map(renameStmt)
        )
    }

    val sources = s0.sources.map { case Source(st, sch, in, sp, ad, loc) => 
      Source(st, renameSchema(sch), in, sp, ad, loc)
    }

    val triggers = s0.triggers.map { t =>
      localMapRenaming = 
        t.stmts.flatMap(getTargetMaps).map(_.name).filterNot(globalMaps.contains)
               .map { n => (n -> (n + "_" + fresh("local"))) }
               .toMap
      Trigger(
        t.event match {
          case EventInsert(s) => EventInsert(renameSchema(s))
          case EventDelete(s) => EventDelete(renameSchema(s))
          case EventBatchUpdate(s) => EventBatchUpdate(renameSchema(s))
          case e => e
        },
        t.stmts.map(renameStmt)
      )
    }
    
    val queries = s0.queries.map { q => Query(q.name, renameExpr(q.expr)) }
    
    System(s0.typeDefs, sources, s0.maps, queries, triggers)
  }

  // 4. Type trigger arguments by binding to the corresponding input schema
  // 5. Type untyped maps definitions (using statements left hand-side) 
  //    (M3 language fix)
  def typeMaps(s0: System) = {
    val schemaMap = s0.sources.map { s => (s.schema.name, s.schema) }.toMap

    def fixSchema(s: Schema): Schema = schemaMap.get(s.name) match {
      case Some(s2) =>
        if (s.fields.nonEmpty && s.fields.map(_._1) != s2.fields.map(_._1)) {
          err("Trigger param names do not match schema field names: " + s + " vs. " + s2)
        }
        s2
      case None => err("Trigger input not found " + s)
    }
    
    val triggers = s0.triggers.map { t => 
      Trigger(t.event match {
        case EventInsert(s) => EventInsert(fixSchema(s))
        case EventDelete(s) => EventDelete(fixSchema(s))
        case EventBatchUpdate(s) => EventBatchUpdate(fixSchema(s))
        case EventReady => EventReady
      }, t.stmts)
    }

    val mapTypes = triggers.flatMap { t =>
      t.stmts.flatMap(getTargetMaps).map(m => (m.name, m.tp))
    }.toMap
   
    val maps = s0.maps.map { m => 
      val t0 = mapTypes.getOrElse(m.name, null)
      val t1 = m.tp
      val tp = if (t0 == null && t1 != null)
                 t1 
               else if (t1 == null && t0 != null)
                 t0
               else if (t0 != t1)
                 sys.error("Map " + m.name + " type differs (" + t0 + " != " + t1 + ")")
               else if (t0 == null)
                 sys.error("Map " + m.name + " has no type")
               else t0
      MapDef(m.name, tp, m.keys, m.expr, m.locality)
    }

    System(s0.typeDefs, s0.sources, maps, s0.queries, triggers)
  }

  // 6. Rename lifted variables to avoid name clashes when code gets flattened.
  // We "lock" input args, output keys and aggregation variables from being renamed
  // Example: Mul(Lift(a,3),Mul(a,Mul(Lift(a,2),a))) => 6
  def renameLifts(s0: System) = {

    def renameExpr(e: Expr, locked: Set[String]): Expr = e.replace {
      case AggSum(ks, sub0) => 
        val ivars = sub0.schema._1
        val lck = locked ++ (ivars ++ ks).map(_._1).toSet
        val sub = renameExpr(sub0, lck)
        val ovars = sub.schema._2.map(_._1).toSet
        val lifts = sub.collect { case Lift(v, _) => List(v) }
        val mapping = lifts.filter(l => ovars.contains(l) && !lck.contains(l))
                           .map((_, fresh("lift"))).toMap
        AggSum(ks, sub.rename(mapping))
    }
    
    def renameStmt(s0: Statement, locked: Set[String] = Set()): Statement = s0 match { 
      case TriggerStmt(target, expr, op, initExpr) =>
        val lck = locked ++ target.keys.map(_._1).toSet
        TriggerStmt(
          target, 
          renameExpr(expr, lck), 
          op, 
          initExpr.map(e => renameExpr(e, lck))
        )
      case IfStmt(cond, thenBlk, elseBlk) =>
        IfStmt(
          Cmp(renameExpr(cond.l, locked), renameExpr(cond.r, locked), cond.op),
          thenBlk.map(s => renameStmt(s, locked)),
          elseBlk.map(s => renameStmt(s, locked))
        )
    }
    
    val triggers = s0.triggers.map { t =>
      val locked = t.event.params.map(_._1).toSet
      Trigger(t.event, t.stmts.map(s => renameStmt(s, locked)))
    }
    
    val queries = s0.queries.map(q => Query(q.name, renameExpr(q.expr, Set())))
    
    freshClear()
    
    System(s0.typeDefs, s0.sources, s0.maps, queries, triggers)
  }

  // 7. Resolve missing types (and also make sure operations are correctly typed)
  def typeCheck(s0: System) = {

    val mapTypes: Map[String, (List[Type], Type)] =
      s0.maps.map { m => (m.name, (m.keys.map(_._2), m.tp)) }.toMap

    def resolveType(tp1: Type, tp2: Type) =
      try { tp1.resolve(tp2) } catch { case _: Throwable => tp2.resolve(tp1) }

    // Change type of Const from String to Char when comparing strings and chars
    // Consequence of using quotes for both strings and chars in SQL/M3
    def demoteString2Char(l: Expr, r: Expr): (Expr, Expr) = (l, r) match {
      case (_, Const(TypeString, vr)) if l.tp == TypeChar =>
        TypeHelper.fromString(vr, TypeChar) // check vr is char
        (l, Const(TypeChar, vr))
      case (Const(TypeString, vl), _) if r.tp == TypeChar =>
        TypeHelper.fromString(vl, TypeChar) // check vl is char
        (Const(TypeChar, vl), r)
      case _ => (l, r)
    }
  
    // give a type to all untyped nodes
    def typeExpr(e0: Expr, ctx0: Map[String, Type]): Expr = 
      try { 
        typeExprCtx(e0, ctx0)._1 
      } catch { 
        case ex: TypeMismatchException => 
          err(ex + " Type mismatch in expression: " + e0) 
      }

    def typeExprCtx(e0: Expr, ctx0: Map[String, Type]): (Expr, Map[String, Type]) = 
      e0 match {
        case Mul(l, r) =>
          val (l2, l2ctx) = typeExprCtx(l, ctx0)
          val (r2, r2ctx) = typeExprCtx(r, l2ctx)
          resolveType(l2.tp, r2.tp)   // check types
          (Mul(l2, r2), r2ctx)
        case Add(l, r) =>
          val (l2, l2ctx) = typeExprCtx(l, ctx0)
          val lfree = l2ctx.filter { case (n, t) => !ctx0.contains(n) }
          val (r2, r2ctx) = typeExprCtx(r, ctx0)
          val rfree = r2ctx.filter { case (n, t) => !ctx0.contains(n) }
          resolveType(l2.tp, r2.tp)   // check types
          (Add(l2, r2), ctx0 ++ lfree ++ rfree)
        case Cmp(l, r, op) =>
          val (l2, l2ctx) = typeExprCtx(l, ctx0)
          val (r2, r2ctx) = typeExprCtx(r, ctx0)
          val (l3, r3) = demoteString2Char(l2, r2)
          resolveType(l3.tp, r3.tp)   // check types
          (Cmp(l3, r3, op), l2ctx ++ r2ctx)
        case CmpOrList(l, rl) =>
          val (l2, l2ctx) = typeExprCtx(l, ctx0)
          val (rl2, rl2ctx) = 
            rl.foldLeft (List.empty[Expr], (ctx0)) { 
              case (acc, r) =>
                val (r2, r2ctx) = typeExprCtx(r, acc._2)
                val (_, r3) = demoteString2Char(l2, r2)
                (acc._1 :+ r3, r2ctx)
            }
          rl2.foreach(r2 => resolveType(l2.tp, r2.tp))  // check types
          (CmpOrList(l2, rl2), l2ctx ++ rl2ctx)  
        case Exists(e) =>    
          val (e2, e2ctx) = typeExprCtx(e, ctx0)
          (Exists(e2), e2ctx)
        case Lift(n, e) =>
          val (e2, e2ctx) = typeExprCtx(e, ctx0)
          val liftCtx = 
            ctx0.get(n) match {
              case Some(t) =>
                try { 
                  resolveType(t, e2.tp) // check types
                  e2ctx
                }
                catch {
                  case _: TypeMismatchException => 
                    err("Value " + n + " lifted as " + t + " compared with " + e2.tp)
                }
              case None =>
                e2ctx + ((n, e2.tp))
            }
          (Lift(n, e2), liftCtx)
        case AggSum(ks, e) =>
          val (e2, e2ctx) = typeExprCtx(e, ctx0)
          val aggCtx = ctx0 ++ ks.map { case (n, _) => (n, e2ctx(n)) }
          val ks2 = ks.map { case (n, _) => (n, aggCtx(n)) }  // resolved types
          (AggSum(ks2, e2), aggCtx)
        case Apply(n, tp, args, targs) => 
          val (args2, ctx2) = args.foldLeft (List.empty[Expr], ctx0) {
            case (acc, e) =>
              val (e2, e2ctx) = typeExprCtx(e, acc._2)
              (acc._1 :+ e2, e2ctx)
          }
          // val tp2 = Library.typeCheck(n, args.map(_.tp))
          (Apply(n, tp, args2, targs), ctx2)
        case Ref(n, t) =>
          (Ref(n, ctx0(n)), ctx0)

        case MapRef(n, tp, ks, tmp, loc) =>
          mapTypes.get(n) match {
            case Some((mktps, mtp)) =>
              val mapCtx = ctx0 ++ ((ks.map(_._1)) zip mktps).toMap
              val mapTp = if (tp == null) mtp else tp
              if (mapTp != mtp) {
                err("Bad value type: expected " + mapTp + ", got " + tp + " for " + e0)
              }
              val mapKeys = (ks zip mktps).map { case ((n, _), t) =>
                if (ctx0.contains(n) && !TypeHelper.cast(ctx0(n), t)) {
                  err("Key type (" + n + ") mismatch in " + e0 + ", tp1: " + ctx0(n) + "  tp2: " + t)
                }
                (n, t)
              }
              (MapRef(n, mapTp, mapKeys, tmp, loc), mapCtx)
            case None => err("Unknown map type: " + e0)
          }
        case Repartition(ks, e) => 
          val (e2, e2ctx) = typeExprCtx(e, ctx0)
          val ks2 = ks.map { case (n, _) => (n, e2ctx(n)) }  // resolved types  
          (Repartition(ks2, e2), e2ctx)        
        case Gather(e) => 
          val (e2, e2ctx) = typeExprCtx(e, ctx0)
          (Gather(e2), e2ctx)
        case _ =>
          (e0, ctx0)
      }
    
    def typeStmt(s0: Statement, ctx: Map[String, Type] = Map(), t: Option[Trigger]): Statement = s0 match {
      case TriggerStmt(target, expr, op, initExpr) =>
        val target2 = typeExpr(target, ctx).asInstanceOf[MapRef]
        val expr2 = typeExpr(expr, ctx)
        val initExpr2 = initExpr.map(e => typeExpr(e, ctx))
        
        def tryCast(e: Expr, target: Expr): Expr = 
          if (e.tp == target.tp || TypeHelper.cast(e.tp, target.tp)) e 
          else err("Type mismatch in LHS " + target.toDecoratedString + " of type " + 
                   target.tp + " and RHS " + e.toDecoratedString + " of type " + e.tp)

        val cExpr = tryCast(expr2, target2)
        val cInitExpr = initExpr2.map(e => tryCast(e, target2))

        TriggerStmt(target2, cExpr, op, cInitExpr)

      case IfStmt(cond, thenBlk, elseBlk) =>
        val cond2 = typeExpr(cond, ctx).asInstanceOf[Cmp]
        thenBlk.map(s => typeStmt(s, ctx, t))
        elseBlk.map(s => typeStmt(s, ctx, t))
        IfStmt(cond2, thenBlk, elseBlk)
    }
    
    val triggers = s0.triggers.map { t =>
      val ctx = t.event.params.toMap
      Trigger(t.event, t.stmts.map(s => typeStmt(s, ctx, Some(t))))
    }
    
    // Set type and keys of each query
    val queries = s0.queries.map { q =>
      val qexpr = typeExpr(q.expr, Map.empty)
      Query(q.name, qexpr)
    }

    System(s0.typeDefs, s0.sources, s0.maps, queries, triggers)
  }

  // 8. Resolve locality information
  def resolveLocality(s0: System): System = {

    val localityMap: Map[String, (List[(String, Type)], LocalityType)] =
      s0.maps.map(m => m.name -> (m.keys, m.locality)).toMap

    def getLocality(n: String, ks: List[(String, Type)]) =
      localityMap(n) match {
        case (_, LocalExp) => Some(LocalExp)
        case (_, DistRandomExp) => Some(DistRandomExp)
        case (refKeys, DistByKeyExp(pk)) => 
          Some(DistByKeyExp(pk.map(k => ks(refKeys.indexOf(k))))) 
      }

    def resolveExpr(e: Expr): Expr = e.replace {
      case m: MapRef =>
        MapRef(m.name, m.tp, m.keys, m.isTemp, getLocality(m.name, m.keys))
      case m: MapRefConst =>
        MapRefConst(m.name, m.keys, getLocality(m.name, m.keys))
      case m: DeltaMapRefConst =>
        MapRefConst(m.name, m.keys, getLocality(m.name, m.keys))
    }

    def resolveStmt(s: Statement): Statement =  s match {
      case TriggerStmt(target, expr, op, initExpr) =>
        val lhs = resolveExpr(target).asInstanceOf[MapRef]
        val rhs = resolveExpr(expr)
        val init = initExpr.map(resolveExpr)

        // Sanity check
        assert(
          (
            lhs.locality.isEmpty || 
            rhs.locality.isEmpty || 
            lhs.locality == rhs.locality) &&
          (init.isEmpty || init.get.locality == lhs.locality),
          "Locality mismatch: " + "\nLHS: " + lhs + 
          "\nRHS: " + rhs + "\nINIT: " + init.mkString
        )
        TriggerStmt(lhs, rhs, op, init)

      case IfStmt(cond, thenBlk, elseBlk) =>
        val rCond = resolveExpr(cond).asInstanceOf[Cmp]
        val rThenBlk = thenBlk.map(resolveStmt)
        val rElseBlk = elseBlk.map(resolveStmt)
        IfStmt(rCond, rThenBlk, rElseBlk)
    }

    val triggers = s0.triggers.map { t =>
      Trigger(t.event, t.stmts.map(resolveStmt))
    }
    
    val queries = s0.queries.map { q =>
      Query(q.name, resolveExpr(q.expr))
    }

    System(s0.typeDefs, s0.sources, s0.maps, queries, triggers)    
  }

  def apply(s: System) = {
    val phases = 
      addMissingMaps _ andThen
      renameVarsAndFuns(
        (s: String) => s.toLowerCase, 
        (s: String) => Map(("/","div")).getOrElse(s, s)
      ) andThen
      typeMaps andThen
      renameLifts andThen
      typeCheck andThen
      resolveLocality

    phases(s)    
  }
}

/*
 * This object exposes user library methods signature to the type checker.
 * Correct referencing/inlining is the responsibility of code generator.
 */
object Library {
  import M3._

  // available functions (overloaded included) as 
  // userName -> (callName, [retType, paramTypes])
  private val functions = 
    new collection.mutable.HashMap[String, (String, List[(Type, List[Type])])]()

  // min # of arguments
  private val minArgs = new collection.mutable.HashMap[String, Int]()

  private def typeOf[T](c: Class[T]) = c.toString match {
    case "char"   => TypeChar
    case "short"  => TypeShort
    case "int"    => TypeInt
    case "long"   => TypeLong
    case "float"  => TypeFloat
    case "double" => TypeDouble
    case "class java.lang.String" => TypeString
    case "class java.util.Date" => TypeDate
    case _ => null
  }

  // Test if name is a library function (used by parser)
  def apply(name: String) = functions.contains(name.toLowerCase)

  def typeCheck(name: String, args: List[Type]): Type = {
    if (!functions.contains(name)) {
      sys.error("Library: no such function: " + name)
    }

    val numArgs = args.size
    val argMin = minArgs.getOrElse(name, 0)
    if (numArgs < argMin) {
      sys.error("Library: not enough argument for " + name + 
                " (expected " + argMin + 
                ", provided " + numArgs + ")")
    }

    val (_, signatures) = functions(name)
    val candidates = signatures.filter(_._2.size >= numArgs)
    if (candidates.isEmpty) {
      sys.error("Library: too many argument for " + name +
                " (expected " + signatures.maxBy(_._2.size) + 
                ", provided " + numArgs + ")")
    } 
    // exact matching  
    candidates.foreach { 
      case (retType, paramTypes) => 
        if ((true /: (args zip paramTypes)) { case (c, (t1, t2)) => c && t1 == t2 })
          return retType
    } 
    // matching with cast
    candidates.foreach { 
      case (retType, paramTypes) => 
        if ((true /: (args zip paramTypes)) { case (c, (t1, t2)) => c && TypeHelper.cast(t1, t2) })
          return retType
    }
    sys.error("Library: bad arguments for " + name + 
              ": got <" + args.mkString(", ") + "> expected " + 
              signatures.map { case (_, paramTypes) => 
                "<" + paramTypes.mkString(", ") + ">" 
              }.mkString(", "))
    null
  }

  private def inspect[T](obj: T, namePrefix: String = null, callPrefix: String = null) {
    val methods = 
      obj.getClass.getMethods
        .filter(m => (m.getModifiers() & java.lang.reflect.Modifier.PUBLIC) != 0)
        .filter(m => namePrefix == null || m.getName.startsWith(namePrefix))

    methods.foreach { m =>
      val retType = typeOf(m.getReturnType)
      val paramTypes = m.getParameterTypes.toList.map(t => typeOf(t))

      if (retType != null && !paramTypes.contains(null)) {
        val name = m.getName
        val userName = if (namePrefix != null) name.substring(namePrefix.length) else name
        val callName = if (callPrefix != null) callPrefix + "." + name else name

        // default argument <i> of <f> is a val named <f>$default$<i> where i=1,2,3...
        val idx = userName.indexOf("$default$")
        if (idx == -1) {
          functions.get(userName) match {
            case Some((cn, signatures)) =>
              // overloaded function
              functions += (userName -> (callName, (retType, paramTypes) :: signatures))
            case None =>
              functions += (userName -> (callName, List((retType, paramTypes))))
          }
        } 
        else { 
          // min #arguments required for this function = min(in-1) forall in
          val fn = userName.substring(0, idx)
          val i = userName.substring(idx + 9).toInt - 1
          
          minArgs += (fn -> Math.min(i, minArgs.getOrElse(fn, i)))
        }
      }
    }
  }

  inspect(fdbresearch.util.Functions, "U")
}
