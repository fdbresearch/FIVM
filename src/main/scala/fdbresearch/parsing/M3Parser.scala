package fdbresearch.parsing

import fdbresearch.core._

/**
  * M3 parser
  */
class M3Parser extends Parser with (String => M3.System) {

  import M3._

  lexical.delimiters ++=
    List("{", "}", ":", ":=", "+", "-", "*", "/", "=", "!=", "<", "<=", ">=", ">", "[", "]", "^=", "+=")

  // ------------ Expressions
  lazy val mapref: Parser[MapRef] =
    ident ~ opt("(" ~> tpe <~ ")") ~ ("[" ~> "]" ~> "[" ~>
      repsep(ident,",") <~ "]") ~ opt(locality) ^^ {
      case n ~ ot ~ ks ~ loc =>
        MapRef(n, ot.orNull, ks.map ((_, null)), isTemp = false,
          if (loc.isEmpty) Some(LocalExp) else loc)
    }

  lazy val locality: Parser[LocalityType] =
    ( "<" ~> "Local" ~> ">" ^^^ LocalExp
    | "<" ~> "DistRandom" ~> ">" ^^^ DistRandomExp
    | "<" ~> "DistByKey(" ~> repsep(ident ~ (":" ~> tpe), ",") <~ ")>" ^^ {
        ks => DistByKeyExp(ks.map { case n ~ t => (n, t) })
      }
    )

  lazy val expr: Parser[Expr] =
    prod ~ opt("+" ~> expr) ^^ { case l ~ or => or match {
      case Some(r) => Add(l, r)
      case None => l
    }}

  lazy val prod: Parser[Expr] =
    atom ~ opt("*" ~> prod) ^^ { case l ~ or => or match {
      case Some(r) => Mul(l, r)
      case None => l
    }}

  lazy val cond: Parser[Cmp] =
    expr ~ ("=" | "!=" | ">" | "<" | ">=" | "<=") ~ expr ^^ {
      case l ~ op ~ r => op match {
        case "="  => Cmp(l, r, OpEq)
        case "!=" => Cmp(l, r, OpNe)
        case ">"  => Cmp(l, r, OpGt)
        case ">=" => Cmp(l, r, OpGe)
        case "<"  => Cmp(r, l, OpGt)
        case "<=" => Cmp(r, l, OpGe)
      }
    }

  lazy val atom: Parser[Expr] = (
    ("AggSum" ~> "(" ~> "[" ~> repsep(ident, ",") <~  "]" <~ ",") ~ expr <~ ")"  ^^ {
        case ks ~ e => AggSum(ks.map((_, null)), e)
      }
    | ("EXISTS"|"DOMAIN") ~> "(" ~> expr <~ ")" ^^ { x => Exists(x) }
    | ("Repartition" ~> "(" ~> "[" ~> repsep(ident, ",") <~  "]" <~ ",") ~ expr <~ ")"  ^^ {
        case ks ~ e => Repartition(ks.map((_, null)), e)
      }
    | "Gather" ~> "(" ~> expr <~ ")"  ^^ { x => Gather(x) }
    | mapref
    | ident ~ ("(" ~> repsep(ident, ",") <~ ")") ^^ {
        case n ~ f => MapRefConst(n, f.map((_, null)))
      } // only in map declaration
    | ("(" ~> "DELTA" ~> ident <~ ")") ~ ("(" ~> repsep(ident, ",") <~ ")") ^^ {
       case n ~ f => DeltaMapRefConst(n, f.map((_, null)))
      }
    | ("[" ~> "/" ~> ":" ~> tpe <~ "]") ~ ("(" ~> expr <~ ")") ^^ {
        case t ~ e => Apply("/", t, List(e))
      }
    | ("[" ~> func <~ ":") ~ (tpe <~ "]") ~ ("(" ~> repsep(expr,",") <~ ")") ^^ {
        case n ~ t ~ as => Apply(n, t, as)
      }
    | "DATE" ~> "(" ~> expr <~ ")" ^^ {
        e => Apply("date", TypeDate, List(e))
      }
    | ("(" ~> ident <~ "^=") ~ (expr <~ ")") ^^ {
        case n ~ v => Lift(n,v)
      }
    | "(" ~> expr <~ ")"
    | "{" ~> (cond | expr) <~ "}"
    | "{" ~> expr ~ ("IN" ~> "[" ~> repsep(expr, ",") <~ "]") <~ "}" ^^ {
        case v ~ consts => CmpOrList(v, consts)
      }
    | ident ^^ { x => Ref(x, null) }
    | floatLit ^^ { Const(TypeFloat, _) }
    | doubleLit ^^ { Const(TypeDouble, _) }
    | longLit ^^ { Const(TypeLong, _) }
    | intLit ^^ { Const(TypeInt, _) }
    | stringLit ^^ { Const(TypeString, _) }
    )

  // ------------ System definition
  lazy val map: Parser[MapDef] =
    ("DECLARE" ~> "MAP" ~> ident) ~ opt("(" ~> tpe <~ ")") ~
      ("[" ~> "]" ~> "[" ~> repsep(ident ~ (":" ~> tpe), ",") <~ "]") ~
      opt(":=" ~> expr) ~ opt(partitioning) <~ ";" ^^ {
        case n ~ t ~ ks ~ e ~ p =>
          MapDef(n, t.orNull, ks.map { case kn ~ kt => (kn, kt) },
            e.orNull, p.getOrElse(LocalExp))
      }

  lazy val query: Parser[Query] =
    ( ("DECLARE" ~> "QUERY" ~> ident <~ ":=") ~ expr <~ ";" ^^ {
        case n ~ e => Query(n, e)
      }
    | failure("Bad M3 query")
    )

  lazy val trigger: Parser[Trigger] =
    ( ("ON" ~> ("+" | "-")) ~ ident ~ ("(" ~> rep1sep(ident, ",") <~ ")") ~
      ("{" ~> rep(stmt) <~ "}") ^^ {
        case op ~ n ~ f ~ ss =>
          val s = Schema(n, f.map{ (_,null) })
          Trigger(if (op == "+") EventInsert(s) else EventDelete(s), ss)
      }
    | "ON" ~> "BATCH" ~> "UPDATE" ~> "OF" ~> ident ~ ("{" ~> rep(stmt) <~ "}") ^^ {
        case n ~ ss => Trigger(EventBatchUpdate(Schema(n, Nil)), ss)
      }
    | "ON" ~> "SYSTEM" ~> "READY" ~> "{" ~> rep(stmt) <~ "}" ^^ {
        Trigger(EventReady, _)
      }
    | failure("Bad M3 trigger")
    )

  lazy val stmt: Parser[Statement] =
    (
      mapref ~ opt(":" ~> "(" ~> expr <~ ")") ~ ("+=" | ":=") ~ expr <~ ";" ^^ {
        case m ~ oi ~ op ~ e =>
          TriggerStmt(m, e, op match { case "+=" => OpAdd case ":=" => OpSet }, oi)
      }
    | "if" ~> ("(" ~> cond <~ ")") ~ ("{" ~> rep(stmt) <~ "}") ~ opt("else" ~> "{" ~> rep(stmt) <~ "}") ^^ {
        case c ~ tss ~ ess =>
          IfStmt(c, tss, ess.getOrElse(Nil))
      }
    )

  lazy val system: Parser[System] =
    rep(typeDef) ~ rep(source) ~ rep(map) ~ rep(query) ~ rep(trigger) ^^ {
      case td ~ ss ~ ms ~ qs ~ ts => System(td, ss, ms, qs, ts)
    }

  def load(path: String) = apply(scala.io.Source.fromFile(path).mkString)

  def apply(str: String): System = phrase(system)(new lexical.Scanner(str)) match {
    case Success(x, _) => x
    case e => sys.error(e.toString)
  }
}