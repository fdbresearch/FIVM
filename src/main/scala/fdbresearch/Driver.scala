package fdbresearch

import fdbresearch.tree.{DTreeNode, Tree, ViewTree}
import fdbresearch.core.SQL
import fdbresearch.parsing.M3Parser

class Driver {

  import fdbresearch.tree.DTree._

  /**
    * Supported SQL format:
    * ----------------------
    * SELECT c1, ..., ck, SUM(f1 * ... * fl)
    * FROM R1 NATURAL JOIN ... NATURAL JOIN RN
    * GROUP BY c1, ..., ck
    *
    * where fi is an (arbitrary) function over attributes.
    */
  // TODO: Add syntax check for admissible SQL queries
  private def getSelectQuery(sql: SQL.System): SQL.Select = sql.queries match {
    case (s @ SQL.Select(false, _, _, None, _, None)) :: Nil => s
    case _ => sys.error("Unsupported SQL query")
  }

  private def getFreeVars(query: SQL.Select): Set[String] = query match {
    case SQL.Select(false, _, _, None, gb, None) =>
      gb.map(_.fs.map(_.n).toSet).getOrElse(Set.empty)
    case _ => sys.error("Unsupported SQL select statement")
  }

  private def liftProdList(expr: SQL.Expr): List[SQL.Expr] = expr match {
    case SQL.Mul(l, r) => liftProdList(l) ++ liftProdList(r)
    case _ => List(expr)
  }

  private def getSumProductExpressions(query: SQL.Select): List[SQL.Expr] = query match {
    case SQL.Select(false, cs, _, None, _, None) =>
      cs.filter {
        case SQL.Agg(_, SQL.OpSum) => true
        case _ => false
      }
      match {
        case SQL.Agg(f, SQL.OpSum) :: Nil => liftProdList(f)
        case _ => sys.error("# of SUM aggregates must be 1.")
      }
    case _ => sys.error("Unsupported SQL select statement")
  }

  private def checkSchemas(sql: SQL.System, dtree: Tree[DTreeNode]): Unit = {
    // All SQL sources must have consistent schemas with dtree relations
    val relationMap = dtree.getRelations.map { r =>
      r.name -> r.keys.map(v => (v.name, v.tp)).toSet
    }.toMap
    assert(sql.sources.forall { s =>
      s.schema.fields.toSet == relationMap(s.schema.name)
    }, "Inconsistent schemas of input sources in SQL and dtree files\n" +
      sql.sources.flatMap { s =>
        val fields1 = s.schema.fields.toSet
        val fields2 = relationMap(s.schema.name)
        fields1.diff(fields2).union(fields2.diff(fields1))
      }.mkString("\n")
    )
  }

  def compile(sql: SQL.System, dtree: Tree[DTreeNode],
              batchUpdates: Boolean, factorizedOutput: Boolean): String = {
    checkSchemas(sql, dtree)

    Main.logger.debug("CHECK SCHEMAS: OK")

    val select = getSelectQuery(sql)
    val freeVars = getFreeVars(select)
    val terms = getSumProductExpressions(select)

    Main.logger.debug("BUILDING VIEW TREE:")

    val viewtree = ViewTree(dtree, freeVars, terms)

    Main.logger.debug("\n\nVIEW TREE:\n" + viewtree)

    val cg = new CodeGenerator(viewtree, sql.typeDefs, sql.sources, batchUpdates, factorizedOutput)
    val m3 = cg.generateM3
    Main.logger.debug("\n\nORIGINAL M3\n" + m3)

    val optM3 = Optimizer.optimize(m3)
    Main.logger.debug("\n\nOPTIMIZED M3\n" + optM3)

    // test that the output can be parsed by the M3 parser
    val checkedM3 = new M3Parser().apply(optM3.toString)
    Main.logger.debug("M3 SYNTAX CHECKED")

    checkedM3.toString
  }
}
