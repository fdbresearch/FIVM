package fdbresearch.tree

import fdbresearch.core._
import fdbresearch.util.Utils

/**
  * View tree implementation
  *
  * @author Milos Nikolic
  */
case class View( name: String,
                 tp: Type,
                 freeVars: List[DTreeVariable],
                 link: Tree[DTreeNode],
                 terms: List[M3.Expr] ) {
  override def toString: String =
    name + "[" + freeVars.map(_.name).mkString(", ") + "]" +
      "(Type: " + tp + (if (terms.nonEmpty) ", Terms: " +
      terms.mkString(" * ") else "") + ")"
}

object ViewTree {
  import DTree._

  implicit class ViewTreeImp(tree: Tree[View]) {
    def getRelations: List[DTreeRelation] = tree.node.link.getRelations
  }

  implicit class M3ExprImp(expr: M3.Expr) {
    def getVariables: Set[String] = {
      val (ivars, ovars) = expr.schema
      (ivars ++ ovars).map(_._1).toSet
    }

    def isCovered(vars: Set[DTreeVariable]): Boolean =
      getVariables.subsetOf(vars.map(_.name))
  }

  // DTree => ViewTree
  def apply(dtree: Tree[DTreeNode], freeVars: Set[String], terms: List[M3.Expr]): Tree[View] = {
    val dtreeVars = dtree.getVariables
    assert(freeVars.subsetOf(dtreeVars.map(_.name).toSet))

    val variableMap = dtreeVars.map(v => v.name -> v).toMap

    // Build view tree from dtree
    dtree.map2WithPostChildren[View] { (tree, children) =>

      // Compute terms at current node
      val treeVars = tree.getVariables.toSet
      val childVars = tree.children.flatMap(_.getVariables).toSet
      val nodeId = tree.variablePreorderIndex
      val nodeTerms = terms.filter(l =>
        !l.isCovered(childVars) && l.isCovered(treeVars)
      ).map(_.replace {
        // Extend node functions with index
        case M3.Apply(f, tp, as, tas) =>
          M3.Apply(f, tp, as, Some(nodeId.toString :: tas.getOrElse(Nil)))
      })

      // Terms can introduce new free variables
      val otherTerms = terms.filter(!_.isCovered(treeVars))
      val otherTermVars = otherTerms.flatMap(_.getVariables).toSet
      val extraFreeVars = otherTermVars.intersect(treeVars.map(_.name))
      val newFreeVars = freeVars.union(extraFreeVars)

      // Extend keys with free variables of children
      val keys = tree.node.keys.map(_.name).toSet
      val childFreeVars = children.flatMap(_.node.freeVars.map(_.name)).toSet
      val nodeFreeVars = keys.union(newFreeVars.intersect(childFreeVars))
      val viewFreeVars = nodeFreeVars.map(variableMap.apply).toList

      // Construct view name
      val relations = tree.getRelations.map(_.name.head).mkString
      val viewName = Utils.fresh("V_" + tree.node.name + "_" + relations)

      val viewType =
        if (children.isEmpty) tree.node.tp    // Relation
        else (children.map(_.node.tp) ++ nodeTerms.map(_.tp)).reduce(Type.resolve)

      View(viewName, viewType, viewFreeVars, tree, nodeTerms)
    }
  }
}
