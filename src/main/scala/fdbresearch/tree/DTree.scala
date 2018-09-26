package fdbresearch.tree

import fdbresearch.core._

trait DTreeNode {
  def name: String
  def tp: Type
  def keys: List[DTreeVariable]
  override def toString = name + "[" + keys.map(_.name).mkString(", ") + "]"
}

case class DTreeVariable(name: String, tp: Type, keys: List[DTreeVariable]) extends DTreeNode
case class DTreeRelation(name: String, keys: List[DTreeVariable]) extends DTreeNode {
  val tp: Type = TypeInt
}

object DTree {

  implicit class SQLExprImp(expr: SQL.Expr) {

    def getVariables: List[String] =
      expr.collect { case SQL.Field(n, _) => List(n) }

    def isCovered(vars: Set[DTreeVariable]): Boolean =
      expr.getVariables.toSet.subsetOf(vars.map(_.name))
  }

  implicit class DTreeImp(dtree: Tree[DTreeNode]) {
    def getVariables: List[DTreeVariable] =
      dtree.collect { case v: DTreeVariable => v }.toList

    def getRelations: List[DTreeRelation] =
      dtree.collect { case r: DTreeRelation => r }.toList

    def pushLifts(lifts: Set[SQL.Expr]): Tree[(DTreeNode, Set[SQL.Expr])] = {
      dtree.map2[(DTreeNode, Set[SQL.Expr])] { tree =>
        val nodeVars = tree.getVariables.toSet
        val childVars = tree.children.flatMap(_.getVariables).toSet
        (tree.node, lifts.filter(l => !l.isCovered(childVars) && l.isCovered(nodeVars)))
      }
    }
  }
}
