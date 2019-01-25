//===----------------------------------------------------------------------===//
//
// Factorized IVM (F-IVM)
//
// https://fdbresearch.github.io/
//
// Copyright (c) 2018-2019, FDB Research Group, University of Oxford
// 
//===----------------------------------------------------------------------===//
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
      val (in, out) = expr.schema
      (in ++ out).map(_._1).toSet
    }

    def isCovered(vars: Set[DTreeVariable]): Boolean =
      getVariables.subsetOf(vars.map(_.name))
  }

  // DTree => ViewTree
  def apply(dtree: Tree[DTreeNode], freeVars: Set[String],
            sumFuns: List[M3.Expr], whConds: List[M3.Expr]): Tree[View] = {
    val dtreeVars = dtree.getVariables
    assert(freeVars.subsetOf(dtreeVars.map(_.name).toSet))

    val variableMap = dtreeVars.map(v => v.name -> v).toMap

    // Build view tree from dtree
    dtree.map2WithPostChildren[View] { (tree, children) =>

      // All available variables at current node
      val availableVars = tree.node match {
        case _: DTreeVariable => children.flatMap(_.node.freeVars).distinct
        case r: DTreeRelation => r.keys
      }

      // Find where conditions at current node. Push conditions down in the tree
      val (nodeWhTerms, restWhTerms) = {
        val childRelationKeys = children.map(_.getRelations.flatMap(_.keys).toSet)
        whConds.filter(w => !childRelationKeys.exists(w.isCovered))
          .partition(_.isCovered(availableVars.toSet))
      }

      // Find SUM functions at current node. Push functions up in the tree
      val (nodeSumTerms, restSumTerms) = {
        val nodeDTreeVars = tree.getVariables.toSet
        val childDTreeVars = tree.children.flatMap(_.getVariables).toSet
        sumFuns.filter(!_.isCovered(childDTreeVars))
          .partition(_.isCovered(nodeDTreeVars))
      }

      // Merge where conditions and sum functions
      val nodeTerms = nodeWhTerms ++ nodeSumTerms

      // Terms can introduce new free variables
      val restTerms = restWhTerms ++ restSumTerms
      val restTermVars = restTerms.flatMap(_.getVariables).toSet
      val newFreeVars = freeVars.union(restTermVars)

      // Extend keys with free variables of children
      val keys = tree.node.keys.map(_.name).toSet
      val nodeFreeVars = keys.union(newFreeVars.intersect(availableVars.map(_.name).toSet))
      val viewFreeVars = // preserve the order of tree variables
        availableVars.map(_.name).filter(nodeFreeVars.contains).map(variableMap.apply)

      // Construct view name
      val relations = tree.getRelations.map(_.name.head).mkString
      val viewName = Utils.fresh("V_" + tree.node.name + "_" + relations)

      // Determine view type
      val viewType = tree.node match {
        case _: DTreeVariable =>
          (children.map(_.node.tp) ++ nodeTerms.map(_.tp)).reduce(Type.resolve)
        case r: DTreeRelation => r.tp
      }

      View(viewName, viewType, viewFreeVars, tree, nodeTerms)
    }
  }
}
