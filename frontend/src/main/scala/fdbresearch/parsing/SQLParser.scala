//===----------------------------------------------------------------------===//
//
// Copyright (c) 2010-2017 EPFL DATA Lab (http://data.epfl.ch)
// 
// Modified by FDB Research Group, University of Oxford
//
// https://fdbresearch.github.io/
//
//===----------------------------------------------------------------------===//
package fdbresearch.parsing

import fdbresearch.core._

/**
  * SQL parser
  */
class SQLParser extends Parser with (String => SQL.System) {

  import SQL._

  lexical.reserved ++=
    List("SELECT", "FROM", "WHERE", "GROUP", "LEFT", "RIGHT",
      "JOIN", "NATURAL", "ON") // reduce this list by conditional accepts

  lexical.delimiters ++=
    List("+", "-", "*", "/", "%", "=", "<>", "!=", "<", "<=", ">=", ">", "[", "]", ":")

  lazy val field: Parser[Field] =
    opt(ident <~ ".") ~ (ident | "*") ^^ {
      case t ~ n => Field(n, t, null)
    }

  // ------------ Expressions
  lazy val expr: Parser[Expr] =
    prod ~ rep(("+" | "-") ~ prod) ^^ {
      case a ~ l =>
        (a /: l) {
          case (l, o ~ r) => o match {
            case "+" => Add(l, r)
            case "-" => Sub(l, r)
          }
        }
    }

  lazy val prod: Parser[Expr] =
    atom ~ rep(("*" | "/" | "%") ~ atom) ^^ {
      case a ~ l =>
        (a /: l) {
          case (l, o ~ r) => o match {
            case "*" => Mul(l, r)
            case "/" => Div(l, r)
            case "%" => Mod(l, r)
          }
        }
    }

  lazy val constant: Parser[Const] =
    ( floatLit ^^ { Const(_, TypeFloat) }
    | doubleLit ^^ { Const(_, TypeDouble) }
    | longLit ^^ { Const(_, TypeLong) }
    | intLit ^^ { Const(_, TypeInt) }
    | stringLit ^^ { Const(_, TypeString) }
    )

  lazy val atom: Parser[Expr] =
    ( "COUNT" ~> "(" ~>"DISTINCT" ~> expr <~ ")" ^^ { Agg(_,OpCountDistinct) }
      //| "INTERVAL" ~> stringLit ~
      //  ("year" | "month" | "day" |
      //   "hours" | "minute" | "second") ^^ { case e ~ u =>
      //    Const(e + ";" + u, TypeDate)
      //   }
    | ( "SUM" ^^^ OpSum
      | "AVG" ^^^ OpAvg
      | "COUNT" ^^^ OpCount
      | "MIN" ^^^ OpMin
      | "MAX" ^^^ OpMax
      ) ~ ("(" ~> expr <~ ")") ^^ { case f ~ e => Agg(e, f) }
    | ("ALL" | "SOME") ~ ("(" ~> query <~ ")") ^^ {
        case op ~ e => op match {
          case "ALL"  => All(e)
          case "SOME" => Som(e)
        }
      }
    | ( "DATE"
      | "YEAR"
      | "MONTH"
      | "DAY"
      ) ~ ("(" ~> rep1sep(expr, ",") <~ ")") ^^ {
        case n ~ as => Apply(n.toLowerCase, TypeDate, as)
      }
    | ("SUBSTRING"
      | ("SUBSTR" ^^^ "substring")
      ) ~ ("(" ~> rep1sep(expr, ",") <~ ")") ^^ {
        case n ~ as => Apply(n.toLowerCase, TypeString, as)
      }
    | "[" ~> funcName ~ opt("<" ~> repsep(intLit | ident, ",") <~ ">") ~
        (":" ~> tpe <~ "]") ~ ("(" ~> repsep(expr, ",") <~ ")") ^^ {
        case n ~ tas ~ t ~ as => Apply(n.toLowerCase, t, as, tas)
      }
    | "CASE" ~> rep1(("WHEN" ~> cond) ~ ("THEN" ~> expr)) ~
      ("ELSE" ~> expr) <~ "END" ^^ {
        case ct ~ e => Case(ct.map{ case c ~ t => (c, t) }, e)
      }
    | ("CASE" ~> expr) ~ rep1(("WHEN" ~> expr) ~ ("THEN" ~> expr)) ~
      ("ELSE" ~> expr) <~ "END" ^^ {
        case c ~ vt ~ e =>
          Case(vt.map { case v ~ t => (Cmp(c, v, OpEq), t) }, e)
      }
    | ( ("DATE_PART" ~> "(" ~> stringLit) ~ ("," ~> expr <~ ")")
      | ("EXTRACT" ~> "(" ~> ident) ~ ("FROM" ~> expr <~ ")")
      ) ^^ { case p ~ e => Apply(p.toLowerCase, TypeInt, List(e)) }
    | field
    | "(" ~> expr <~ ")"
    | "(" ~> query <~ ")" ^^ Nested
    | constant
    | failure("SQL expression")
    )

  // ------------ Conditions
  def disj: Parser[Cond] =
    rep1sep(conj, "OR") ^^ { cs => (cs.head /: cs.tail) ((x, y) => Or(x, y)) }

  def conj: Parser[Cond] =
    rep1sep(cond, "AND") ^^ { cs => (cs.head /: cs.tail) ((x, y) => And(x, y)) }

  lazy val cond: Parser[Cond] =
    ( "EXISTS" ~> "(" ~> query <~ ")" ^^ Exists
    | "NOT" ~> cond ^^ Not
    | expr ~ ("INLIST" ~> "(" ~> rep1sep(constant, ",") <~ ")") ^^ {
        case e ~ l => InList(e, l)
      }
    | expr ~ opt("NOT") ~ ("LIKE" ~> stringLit) ^^ {
        case e ~ o ~ s => o match {
          case Some(_) => Not(Like(e, s))
          case None => Like(e, s)
      }}
    | expr ~ ("BETWEEN" ~> expr) ~ ("AND" ~> expr) ^^ {
        case e ~ m ~ n => And(Cmp(e, m, OpGe), Cmp(n, e, OpGe))
      }
    | expr ~ (opt("NOT") <~ "IN") ~ query ^^ {
        case e ~ Some(_) ~ q => Not(In(e, q))
        case e ~ None ~ q => In(e,q)
      }
    | expr ~
      ( "=" ^^^ OpEq
      | "<>" ^^^ OpNe
      | ">" ^^^ OpGt
      | ">=" ^^^ OpGe
      | "!=" ^^^ OpNe
      ) ~ expr ^^ {
        case l ~ op ~ r => Cmp(l, r, op)
      }
    | expr ~ ("<" ^^^ OpGt | "<=" ^^^ OpGe) ~ expr ^^ {
        case l ~ op ~ r => Cmp(r, l, op)
      }
    | "(" ~> disj <~ ")"
    | failure("SQL condition")
    )

  // ------------ Queries
  lazy val query: Parser[Query] =
    qconj ~ opt("UNION" ~> opt("ALL") ~ query) ^^ {
      case q1 ~ Some(a ~ q2) => Union(q1, q2, a.isDefined)
      case q1 ~ None => q1
    }

  lazy val qconj: Parser[Query] =
    qatom ~ opt("INTERSECT" ~> qconj) ^^ {
      case q1 ~ Some(q2) => Inter(q1,q2)
      case q1 ~ None => q1
    }

  lazy val tab: Parser[Table] =
    ( "(" ~> query <~ ")" ^^ TableQuery | ident ^^ TableNamed) ~
      opt(opt("AS") ~> ident) ^^ {
      case t ~ Some(n) => TableAlias(t, n)
      case t ~ None => t
    }

  lazy val join: Parser[Table] =
    tab ~
      rep( // joins should be left-associative
        ("NATURAL" ~ "JOIN") ~> tab ^^ { (_, JoinInner, None) }
        | (opt(  "LEFT" ^^^ JoinLeft
               | "RIGHT" ^^^ JoinRight
               | "FULL" ^^^ JoinFull) <~ opt("OUTER") <~ "JOIN") ~
          tab ~ ("ON" ~> cond) ^^ {
            case j ~ t ~ c => (t, j.getOrElse(JoinInner), Some(c))
          }
      ) ^^ {
        case t ~ js => (t /: js) {
          case (t1, (t2, j, c)) => TableJoin(t1, t2, j, c)
        }
      }

  lazy val alias: Parser[Expr] =
    expr ~ opt("AS" ~> ident) ^^ {
      case e ~ o => o match {
        case Some(n) => Alias(e, n)
        case None => e
      }
    }

  lazy val groupBy: Parser[GroupBy] =
    "GROUP" ~> "BY" ~> rep1sep(field, ",") ~ opt("HAVING" ~> disj) ^^ {
      case fs ~ ho => GroupBy(fs, ho)
    }

  lazy val orderBy: Parser[OrderBy] =
    "ORDER" ~> "BY" ~> rep1sep(field ~ opt("ASC" | "DESC"), ",") ^^ {
      fs => OrderBy(fs.map {
        case f ~ o => (f, o.getOrElse("").toUpperCase == "DESC")
      })
    }

  lazy val qatom: Parser[Query] =
    ( select
    | opt("LIST") ~> "(" ~> repsep(expr, ",") <~ ")" ^^ { x => Lst(x) }
    | "(" ~> query <~ ")"
    )

  lazy val select: Parser[Select] =
    ("SELECT" ~> opt("DISTINCT")) ~ rep1sep(alias, ",") ~
      opt("FROM" ~> repsep(join, ",")) ~
      opt("WHERE" ~> disj) ~
      opt(groupBy) ~
      opt(orderBy) ^^ {
      case d ~ cs ~ ts ~ wh ~ gb ~ ob =>
        Select(d.isDefined, cs, ts.getOrElse(Nil), wh, gb, ob)
    }

  // ------------ System definition
  lazy val system: Parser[System] =
    opt(variableOrder) ~ rep(typeDef) ~ rep(source) ~ rep(select <~ opt(";")) ^^ {
      case vo ~ td ~ ss ~ qs => System(vo, td, ss, qs)
    }

  def apply(str: String): System = phrase(system)(new lexical.Scanner(str)) match {
    case Success(x, _) => x
    case e => sys.error(e.toString)
  }

  def load(path: String, base: Option[String]): System = {
    def f(p: String) =
      scala.io.Source.fromFile(
        base.map(_ + "/" + p).getOrElse(p)
      ).mkString

    apply("(?i)INCLUDE [\"']?([^\"';]+)[\"']?;".r.replaceAllIn(
      f(path), m => f(m.group(1))).trim
    )
  }
}
