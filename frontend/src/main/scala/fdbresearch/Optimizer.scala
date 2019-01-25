//===----------------------------------------------------------------------===//
//
// Factorized IVM (F-IVM)
//
// https://fdbresearch.github.io/
//
// Copyright (c) 2018-2019, FDB Research Group, University of Oxford
// 
//===----------------------------------------------------------------------===//
package fdbresearch

import fdbresearch.core._
import fdbresearch.util.{Logger, Utils}

object Optimizer {

  object PartialEvaluator extends (M3.Expr => M3.Expr) {

    implicit class ConstExtensions(c1: M3.Const) {
      val zero = M3.Const(TypeChar, "0")
      val one = M3.Const(TypeChar, "1")

      def *(c2: M3.Const): M3.Const = Type.resolve(c1.tp, c2.tp) match {
        case tp @ (TypeChar | TypeShort | TypeInt | TypeLong) =>
          val v = c1.v.toLong * c2.v.toLong
          M3.Const(tp, v.toLong.toString)
        case tp @ (TypeFloat | TypeDouble) =>
          val v = BigDecimal(c1.v) * BigDecimal(c2.v)
          M3.Const(tp, v.toDouble.toString)
        case _ => sys.error("[M3 Optimizer] Const multiplication failed")
      }

      def +(c2: M3.Const): M3.Const = Type.resolve(c1.tp, c2.tp) match {
        case tp @ (TypeChar | TypeShort | TypeInt | TypeLong) =>
          val v = c1.v.toLong + c2.v.toLong
          M3.Const(tp, v.toLong.toString)
        case tp @ (TypeFloat | TypeDouble) =>
          val v = BigDecimal(c1.v) + BigDecimal(c2.v)
          M3.Const(tp, v.toDouble.toString)
        case _ => sys.error("[M3 Optimizer] Const addition failed")
      }

      def isEqual(c2: M3.Const): M3.Const = if (eq(c1, c2)) one else zero

      def isNotEqual(c2: M3.Const): M3.Const = if (!eq(c1, c2)) one else zero

      def >(c2: M3.Const): M3.Const = if (gt(c1, c2)) one else zero

      def >=(c2: M3.Const): M3.Const = if (gt(c1, c2) || eq(c1, c2)) one else zero

      private def eq(c1: M3.Const, c2: M3.Const) = c1 == c2

      private def gt(c1: M3.Const, c2: M3.Const) = Type.resolve(c1.tp, c2.tp) match {
        case TypeChar | TypeShort | TypeInt | TypeLong | TypeDate => c1.v.toLong > c2.v.toLong
        case TypeFloat | TypeDouble => BigDecimal(c1.v) > BigDecimal(c2.v)
        case TypeString => c1.v > c2.v
        case _ => sys.error("[M3 Optimizer] Comparison failed")
      }
    }

    def eval(e: M3.Expr): M3.Expr = e.replace {
      case M3.Mul(l, r) => (eval(l), eval(r)) match {
        case (c1: M3.Const, c2: M3.Const) => c1 * c2
        case (l2, r2) => M3.Mul(l2, r2)
      }
      case M3.Add(l, r) => (eval(l), eval(r)) match {
        case (c1: M3.Const, c2: M3.Const) => c1 + c2
        case (l2, r2) => M3.Add(l2, r2)
      }
      case M3.Cmp(l, r, op) => (eval(l), eval(r)) match {
        case (c1: M3.Const, c2: M3.Const) =>
          op match {
            case OpEq => c1 isEqual c2
            case OpNe => c1 isNotEqual c2
            case OpGt => c1 > c2
            case OpGe => c2 >= c2
          }
        case (l2, r2) => M3.Cmp(l2, r2, op)
      }
    }

    def apply(e: M3.Expr): M3.Expr = eval(e)
  }

  object Rewriter extends (M3.Expr => M3.Expr) {

    // TODO: remove unnecessary group-by variables from AggSums
    //       requires scope and schema in fold

    def rewrite(e: M3.Expr): M3.Expr = e.replace {
      case M3.AggSum(ks1, e1) => rewrite(e1) match {
        case M3.AggSum(_, e2) => rewrite(M3.AggSum(ks1, e2)) // unnest AggSum
        case e2 => M3.AggSum(ks1, e2)
      }
      case M3.Mul(l, r) => (rewrite(l), rewrite(r)) match {
        case (M3.AggSum(ks1, e1), e2) =>
          // Find product terms covered by ks1 and without maps in e2
          val (covered, rest) = prodList(e2).partition { t =>
            val (tin, tout) = t.schema
            (tin ++ tout).toSet.subsetOf(ks1.toSet) &&     // singleton
              !findMapsInExpr(t).exists(_.keys.nonEmpty)   // no maps (expensive lookups)
          }
          if (covered.isEmpty) M3.Mul(M3.AggSum(ks1, e1), e2)
          else {
            val subexp = (prodList(e1) ++ covered).reduceLeft(M3.Mul)
            val newexp = (M3.AggSum(ks1, subexp) :: rest).reduceLeft(M3.Mul)
            rewrite(newexp)
          }
        case (e1, e2) => M3.Mul(e1, e2)
      }
    }

    def apply(e: M3.Expr): M3.Expr = rewrite(e)
  }

  def optimize(m3: M3.System): M3.System = {
    Logger.instance.debug("[OPTIMIZER]: System")

    val (newTriggers, newMaps) = m3.triggers.map(optimizeTrigger).unzip
    M3.System(m3.typeDefs, m3.sources, m3.maps ++ newMaps.flatten, m3.queries, newTriggers)
  }

  private def optimizeTrigger(t: M3.Trigger): (M3.Trigger, List[M3.MapDef]) = {
    Logger.instance.debug("[OPTIMIZER]: Trigger")

    def optimizeStmts(stmts: List[M3.Statement], maps: List[M3.MapDef])
        : (List[M3.Statement], List[M3.MapDef]) = {
      val (optStmts, newMap) = optCSE(stmts)
      if (stmts == optStmts) (stmts, maps)
      else optimizeStmts(optStmts, maps ++ newMap)
    }
    val (stmts, newMaps) = optimizeStmts(t.stmts.map(optimizeStatement), Nil)
    (M3.Trigger(t.event, stmts), newMaps)
  }

  private def optimizeStatement(s: M3.Statement): M3.Statement = s match {
    case M3.TriggerStmt(t, e, op, ie) =>
      M3.TriggerStmt(t, optimizeExpr(e), op, ie.map(optimizeExpr))
    case M3.IfStmt(cond, thenBlk, elseBlk) =>
      M3.IfStmt(cond, thenBlk.map(optimizeStatement), elseBlk.map(optimizeStatement))
  }

  def optimizeExpr(e: M3.Expr): M3.Expr = {
    Logger.instance.debug("[OPTIMIZE EXPR]: Before " + e)
    val oe = (PartialEvaluator andThen Rewriter)(e)
    Logger.instance.debug("[OPTIMIZE EXPR]: After " + oe)
    oe
  }

  private def findMapsInExpr(e: M3.Expr): List[M3.MapRef] =
    e.collect { case m: M3.MapRef => List(m) }

  private def prodList(e: M3.Expr): List[M3.Expr] = e match {
    case M3.Mul(l, r) => prodList(l) ++ prodList(r)
    case _ => List(e)
  }

  // CSE across statements, works with AggSums only
  private def optCSE(ss: List[M3.Statement]): (List[M3.Statement], Option[M3.MapDef]) = {
    Logger.instance.debug("[OPTIMIZE CSE]: Before " + ss.mkString("\n"))

    // Replace src in statements with dst
    def replace(ss: List[M3.Statement], src: M3.Expr, dst: M3.MapRef): List[M3.Statement] = {
      def replaceInExpr(e: M3.Expr): M3.Expr = e.replace {
        case a: M3.AggSum => src.cmp(a) match {
          case Some(mapping) => M3.MapRef(dst.name, dst.tp, dst.keys.map(mapping), isTemp = true)
          case None => M3.AggSum(a.keys, replaceInExpr(a.e))
        }
      }
      def replaceInStatement(s: M3.Statement): M3.Statement = s match {
        case M3.TriggerStmt(target, e, op, ie) =>
          M3.TriggerStmt(target, replaceInExpr(e), op, ie.map(replaceInExpr))
        case M3.IfStmt(c, thenBlk, elseBlk) =>
          M3.IfStmt(c, thenBlk.map(replaceInStatement), elseBlk.map(replaceInStatement))
      }
      ss.map(replaceInStatement)
    }

    // Return all AggSum in statements
    def findAggregates(ss: List[M3.Statement]): List[M3.AggSum] = {
      def findInExpr(e: M3.Expr): List[M3.AggSum] =
        e.collect { case a: M3.AggSum => a :: findInExpr(a.e) }

      def findInStatement(s: M3.Statement): List[M3.AggSum] = s match {
        case M3.TriggerStmt(_, e, _, ie) =>
          findInExpr(e) ++ ie.map(findInExpr).getOrElse(Nil)
        case M3.IfStmt(_, thenBlk, elseBlk) =>
          findAggregates(thenBlk) ++ findAggregates(elseBlk)
      }
      ss.flatMap(findInStatement)
    }

    // Check if MapRef appears in statement
    def isReferencingMap(s: M3.Statement, m: M3.MapRef): Boolean = {
      def isMapRefInExpr(e: M3.Expr): Boolean =
        e.collect { case m2: M3.MapRef => List(m == m2) }.exists(identity)

      def isMapRefInStatement(s: M3.Statement): Boolean = s match {
        case M3.TriggerStmt(_, e, _, ie) =>
          isMapRefInExpr(e) || ie.exists(isMapRefInExpr)
        case M3.IfStmt(_, thenBlk, elseBlk) =>
          thenBlk.map(isMapRefInStatement).exists(identity) ||
            elseBlk.map(isMapRefInStatement).exists(identity)
      }
      isMapRefInStatement(s)
    }

//    def sortFn(e1: (M3.AggSum, Int), e2: (M3.AggSum, Int)): Boolean = {
//      def appearsIn(src: M3.AggSum, dst: M3.Expr): Boolean =
//        dst.collect {
//          case m: M3.AggSum => List(m == src || appearsIn(src, m.e))
//        }.exists(identity)
//      (e1._2 > e2._2) || (e1._2 == e2._2 && appearsIn(e2._1, e1._1))
//    }

    // Find candidates, ranked them by # of occurrences, replace top expression if > 1
    val candidates = findAggregates(ss)
    val ranked = candidates.groupBy(identity).mapValues(_.size).toList.sortWith(_._2 > _._2)
    ranked match {
      case (a, occurrences) :: _ if occurrences > 1 =>
        val tmpMapDef = M3.MapDef(Utils.fresh("TMP_SUM"), a.tp, a.keys, optimizeExpr(a))
        val tmpMapRef = M3.MapRef(tmpMapDef.name, tmpMapDef.tp, tmpMapDef.keys, isTemp = true)

        // Replace expression 'a' by tmpMapRef
        val replacedStmts = replace(ss, a, tmpMapRef)

        // Compute expression 'a' before first statement using tmpMapRef
        val indexFirst = replacedStmts.indexWhere(s => isReferencingMap(s, tmpMapRef))
        val stmts = replacedStmts.take(indexFirst) ++
          (M3.TriggerStmt(tmpMapRef, a, M3.OpSet, None) :: replacedStmts.drop(indexFirst))

        Logger.instance.debug("[OPTIMIZE CSE]: After " + stmts.mkString("\n"))

        (stmts, Some(tmpMapDef))

      case _ => (ss, None)
    }
  }
}
