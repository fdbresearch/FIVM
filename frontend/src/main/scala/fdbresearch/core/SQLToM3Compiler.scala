//===----------------------------------------------------------------------===//
//
// Factorized IVM (F-IVM)
//
// https://fdbresearch.github.io/
//
// Copyright (c) 2018-2019, FDB Research Group, University of Oxford
// 
//===----------------------------------------------------------------------===//
package fdbresearch.core

import fdbresearch.core.SQL.Cond.pushdownNots
import fdbresearch.util.Utils

/**
  * Supported SQL format:
  * ----------------------
  * SELECT c1, ..., ck, SUM(f1 * ... * fl)
  * FROM R1 NATURAL JOIN ... NATURAL JOIN RN
  * WHERE cond
  * GROUP BY c1, ..., ck
  *
  * where fi is an (arbitrary) function over attributes.
  */
object SQLToM3Compiler {

  final case class UnsupportedSQLException(
      private val msg: String = "",
      private val cause: Throwable = None.orNull)
    extends Exception(msg, cause)

  private def typeOf(e: SQL.Expr): Type = e match {
    case SQL.Field(_, _, tp) => tp
    case SQL.Const(_, tp) => tp
    case SQL.Apply(_, tp, _, _) => tp
    case SQL.Add(l, r) => Type.resolve(typeOf(l), typeOf(r))
    case SQL.Sub(l, r) => Type.resolve(typeOf(l), typeOf(r))
    case SQL.Mul(l, r) => Type.resolve(typeOf(l), typeOf(r))
    case SQL.Div(l, r) => Type.resolve(typeOf(l), typeOf(r))
    case SQL.Mod(l, r) => Type.resolve(typeOf(l), typeOf(r))
    case _ => throw UnsupportedSQLException("[SQL] Not supported expression: " + e)
  }

  private def compile(e: SQL.Expr): M3.Expr = e match {
    case SQL.Field(n, _, tp) if n != "*" => M3.Ref(n, tp)
    case SQL.Const(v, tp) => M3.Const(tp, v)
    case SQL.Apply(f, tp, as, tas) => M3.Apply(f, tp, as.map(compile), tas)
    case SQL.Add(l, r) => M3.Add(compile(l), compile(r))
    case SQL.Sub(l, r) => M3.Add(compile(l), M3.Mul(M3.Const(TypeInt, "-1"), compile(r)))
    case SQL.Mul(l, r) => M3.Mul(compile(l), compile(r))
    case SQL.Div(l, r) => M3.Apply("div", typeOf(e), List(compile(l), compile(r)))
    case SQL.Mod(l, r) => M3.Apply("mod", typeOf(e), List(compile(l), compile(r)))
    case SQL.Case(hd :: tl, d) =>
      M3.Add(
        M3.Mul(compile(hd._1), compile(hd._2)),
        M3.Mul(compile(SQL.Not(hd._1)), compile(SQL.Case(tl, d)))
      )
    case SQL.Case(Nil, d) => compile(d)
    case SQL.Alias(e1, n) => M3.Lift(n, compile(e1))
    case _ => throw UnsupportedSQLException("[SQL] Not supported expression: " + e)
  }

  private def compile(c: SQL.Cond): M3.Expr = SQL.Cond.pushdownNots(c) match {
    case SQL.And(l, r) => M3.Mul(compile(l), compile(r))
    case SQL.Or(l, r) =>
      val name = Utils.fresh("or_lift")
      val lift = M3.Lift(name, M3.Add(compile(l), compile(r)))
      val cmp = M3.Cmp(M3.Ref(lift.name, lift.tp), M3.Const.Zero, OpGt)
      M3.AggSum(Nil, M3.Mul(lift, cmp))
    case SQL.Not(c1) =>
      val name = Utils.fresh("not_lift")
      val lift = M3.Lift(name, compile(c1))
      val cmp = M3.Cmp(M3.Ref(lift.name, lift.tp), M3.Const.Zero, OpEq)
      M3.AggSum(Nil, M3.Mul(lift, cmp))
    case SQL.Cmp(l, r, op) => M3.Cmp(compile(l), compile(r), op)
    case SQL.InList(e, l) => M3.CmpOrList(compile(e), l.map(compile))
    case SQL.Like(e, l) =>
      val matchStr = l.replace("%", ".*")
      val matchConst = M3.Const(TypeString, s"^${matchStr}$$")
      M3.Cmp(
        M3.Apply("regexp_match", TypeInt, List(matchConst, compile(e))),
        M3.Const.Zero,
        OpNe
      )
    case _ => throw UnsupportedSQLException("[SQL] Not supported condition: " + c)
  }

  private def extractSumAggFn(cs: List[SQL.Expr]): M3.Expr = {
    val aggs = cs.collect { case a: SQL.Agg => a }
    require(aggs match {
      case Seq(SQL.Agg(_, SQL.OpSum)) => true
      case _ => false
    }, "[SQL] Number of SUM aggregates must be 1")

    cs.flatMap {
      case SQL.Agg(e, SQL.OpSum) => List(compile(e))
      case e: SQL.Alias => List(compile(e))
      case e: SQL.Field if e.n != "*" => compile(e); Nil   // compile and discard
      case e => List(M3.Lift(Utils.fresh("col"), compile(e)))
    }.reduce(M3.Mul.apply)
  }

  private def extractTableNames(t: SQL.Table): List[String] = t match {
    case SQL.TableJoin(t1, t2, SQL.JoinInner, None) =>
      extractTableNames(t1) ++ extractTableNames(t2)
    case SQL.TableNamed(n) => List(n)
    case _ => throw UnsupportedSQLException("[SQL] Only natural joins are supported")
  }

  private def extractTableNames(join: List[SQL.Table]): List[String] = join match {
    case t :: Nil => extractTableNames(t)
    case _ => throw UnsupportedSQLException("[SQL] Only natural joins are supported")
  }

  private def extractGroupByVars(gb: SQL.GroupBy, colMapping: Map[M3.Expr, String]): List[String] = gb match {
    case SQL.GroupBy(fs, None) => fs.map {
        case f: SQL.Field => f.n
        case e => colMapping(compile(e))
      }
    case _ => throw UnsupportedSQLException("[SQL] Not supported group by statement")
  }

  private def toList(e: M3.Expr): List[M3.Expr] = e match {
    case M3.Mul(l, r) => toList(l) ++ toList(r)
    case _ => List(e)
  }

  def compile(sys: SQL.System): (List[M3.Expr], List[String], List[M3.Expr], List[String]) =
    sys.queries match {
      case SQL.Select(false, cs, ts, wh, gb, None) :: Nil =>
        val sumFn = toList(extractSumAggFn(cs))
        val colMapping = sumFn.collect { case l: M3.Lift => l.e -> l.name }.toMap
        val tables = extractTableNames(ts)
        val whCond = wh.map(compile).map(toList).getOrElse(Nil)
        val gbVars = gb.map(g => extractGroupByVars(g, colMapping)).getOrElse(Nil)
        (sumFn, tables, whCond, gbVars)
      case _ => throw UnsupportedSQLException("[SQL] Not supported SQL query")
    }
}