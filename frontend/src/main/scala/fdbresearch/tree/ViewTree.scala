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

case class Variable(name: String, tp: Type)

case class View(name: String,
                tp: Type,
                freeVars: List[Variable],
                link: Tree[VariableOrderNode],
                terms: List[M3.Expr] ) {
  override def toString: String =
    name + "[" + freeVars.map(_.name).mkString(", ") + "]" +
      "(Type: " + tp + (if (terms.nonEmpty) ", Terms: " +
      terms.mkString(" * ") else "") + ")"
}

object ViewTree {
  import VariableOrder._

  implicit class ViewTreeImp(tree: Tree[View]) {
    def getRelations: List[VariableOrderRelation] = tree.node.link.getRelations

    def getTerms: List[M3.Expr] = tree.collect { case v: View => v.terms }.flatten.toList
  }

  implicit class M3ExprImp(expr: M3.Expr) {
    def getVariables: Set[String] = {
      val (in, out) = expr.schema
      (in ++ out).map(_._1).toSet
    }

    def getInputVariables: Set[String] =
      expr.schema._1.map(_._1).toSet

    def isCovered(vars: Set[Variable]): Boolean =
      getInputVariables.subsetOf(vars.map(_.name))
  }

  // DTree => ViewTree
  def apply(dtree: Tree[VariableOrderNode], freeVars: Set[String],
            sumFuns: List[M3.Expr], whConds: List[M3.Expr]): Tree[View] = {
    val dtreeVars = dtree.getVariables.map(v => Variable(v.name, v.tp))
    val liftVars = sumFuns.collect { case l: M3.Lift => Variable(l.name, l.e.tp) }

    require({
      val dtreeVarNames = dtreeVars.map(_.name)
      val liftVarNames = liftVars.map(_.name)
      dtreeVarNames.intersect(liftVarNames).isEmpty
    }, "Duplicate column names found in both schema and aliases")

    val allVars = dtreeVars ++ liftVars
    require(
      freeVars.subsetOf(allVars.map(_.name).toSet),
      "Output column names not found"
    )
    val variableMap = allVars.map(v => v.name -> v).toMap

    // Build view tree from dtree
    dtree.map2WithPostChildren[View] { (tree, children) =>

      // All available variables at current node
      val availableVars = tree.node match {
        case _: VariableOrderVar => children.flatMap(_.node.freeVars).distinct
        case r: VariableOrderRelation => r.keys.map(k => Variable(k.name, k.tp))
      }

      // Find where conditions at current node. Push conditions down in the tree
      val (nodeWhTerms, restWhTerms) = {
        val childRelationKeys = children.map(c =>
          c.getRelations.flatMap(r =>
            r.keys.map(k => Variable(k.name, k.tp))
          ).toSet
        )
        whConds.filter(w => !childRelationKeys.exists(w.isCovered))
          .partition(_.isCovered(availableVars.toSet))
      }

      // Find SUM functions at current node. Push functions up in the tree
      val (nodeSumTerms, restSumTerms) = {
        val nodeVars = tree.getVariables.map(v => Variable(v.name, v.tp)).toSet
        val childVars = tree.children.flatMap(_.getVariables).map(v => Variable(v.name, v.tp)).toSet
        sumFuns.filter(f =>
          !f.isCovered(childVars) ||
            (tree.parent.isEmpty && !children.exists(_.getTerms.contains(f)))
        ).partition(_.isCovered(nodeVars))
      }

      val liftVars = nodeSumTerms.collect { case l: M3.Lift => Variable(l.name, l.e.tp) }
      val availableVars2 = availableVars ++ liftVars

      // Merge where conditions and sum functions
      val nodeTerms = nodeWhTerms ++ nodeSumTerms

      // Terms can introduce new free variables
      val restTerms = restWhTerms ++ restSumTerms
      val restTermVars = restTerms.flatMap(_.getVariables).toSet
      val newFreeVars = freeVars.union(restTermVars)

      if (tree.parent.isEmpty) {
        // Sanity checks
        assert(restTerms.isEmpty)
        val usedTerms = nodeTerms ++ children.flatMap(_.getTerms)
        assert(sumFuns.forall(e => usedTerms.contains(e)))
        assert(whConds.forall(e => usedTerms.contains(e)))
      }

      // Extend keys with free variables of children
      val keys = tree.getKeys.map(_.name).toSet
      val nodeFreeVars = keys.union(newFreeVars.intersect(availableVars2.map(_.name).toSet))
      val viewFreeVars = // preserve the order of tree variables
        availableVars2.map(_.name).filter(nodeFreeVars.contains).map(variableMap.apply)

      // Construct view name
      val suffix = tree.getRelations.map(_.name.head).mkString
      val viewName = Utils.fresh("V_" + tree.node.name + "_" + suffix)

      // Determine view type
      val viewType = tree.node match {
        case _: VariableOrderVar =>
          (children.map(_.node.tp) ++ nodeTerms.map(_.tp)).reduce(Type.resolve)
        case r: VariableOrderRelation => r.tp
      }

      View(viewName, viewType, viewFreeVars, tree, nodeTerms)
    }
  }
}
