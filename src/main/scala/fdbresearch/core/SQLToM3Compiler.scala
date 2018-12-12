package fdbresearch.core

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
    case SQL.Apply(_, tp, _) => tp
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
    case SQL.Apply(f, tp, as) => M3.Apply(f, tp, as.map(compile))
    case SQL.Add(l, r) => M3.Add(compile(l), compile(r))
    case SQL.Sub(l, r) => M3.Add(compile(l), M3.Mul(M3.Const(TypeChar, "-1"), compile(r)))
    case SQL.Mul(l, r) => M3.Mul(compile(l), compile(r))
    case SQL.Div(l, r) => M3.Apply("div", typeOf(e), List(compile(l), compile(r)))
    case SQL.Mod(l, r) => M3.Apply("mod", typeOf(e), List(compile(l), compile(r)))
    case _ => throw UnsupportedSQLException("[SQL] Not supported expression: " + e)
  }

  private def compile(c: SQL.Cond): M3.Expr = c match {
    case SQL.And(l, r) => M3.Mul(compile(l), compile(r))
    case SQL.Cmp(l, r, op) => M3.Cmp(compile(l), compile(r), op)
    case _ => throw UnsupportedSQLException("[SQL] Not supported condition: " + c)
  }

  private def extractSumAggFn(cs: List[SQL.Expr]): M3.Expr =
    cs.flatMap {
      case SQL.Agg(e, SQL.OpSum) => List(compile(e))
      case e => compile(e); Nil   // compile and discard other fields
    } match {
      case hd :: Nil => hd
      case _ => throw UnsupportedSQLException("[SQL] Number of SUM aggregates must be 1")
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

  private def extractGroupByVars(gb: SQL.GroupBy): List[String] = gb match {
    case SQL.GroupBy(fs, None) => fs.map(_.n)
    case _ => throw UnsupportedSQLException("[SQL] Not supported group by statement")
  }

  private def toList(e: M3.Expr): List[M3.Expr] = e match {
    case M3.Mul(l, r) => toList(l) ++ toList(r)
    case _ => List(e)
  }

  def compile(sys: SQL.System): (List[M3.Expr], List[String], List[String]) =
    sys.queries match {
      case SQL.Select(false, cs, ts, wh, gb, None) :: Nil =>
        val sumFn = extractSumAggFn(cs)
        val tables = extractTableNames(ts)
        val whCond = wh.map(compile)
        val gbVars = gb.map(extractGroupByVars).getOrElse(Nil)
        // Concatenate where and sum terms
        val terms = whCond.map(toList).getOrElse(Nil) ++ toList(sumFn)
        (terms, tables, gbVars)
      case _ => throw UnsupportedSQLException("[SQL] Not supported SQL query")
    }
}