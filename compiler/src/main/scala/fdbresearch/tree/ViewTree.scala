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
    def getInputVariables: Set[String] =
      expr.schema._1.map(_._1).toSet

    def getOutputVariables: Set[String] =
      expr.schema._2.map(_._1).toSet

    def isCovered(vars: Set[String]): Boolean = getInputVariables.subsetOf(vars)
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
      val availableVarList = tree.node match {
        case _: VariableOrderVar => children.flatMap(_.node.freeVars.map(_.name)).distinct
        case r: VariableOrderRelation => r.keys.map(_.name)
      }
      val availableVars = availableVarList.toSet

      // Node's dependency set
      val nodeKeys = tree.getKeys.map(_.name).toSet

      // Drop used terms
      val usedTerms = children.flatMap(_.getTerms)
      val remainingSumTerms = sumFuns.diff(usedTerms)
      val remainingWhTerms = whConds.diff(usedTerms)

      // Find SUM terms at current node. Push functions up in the tree
      val (nodeSumTerms, restSumTerms) = {
        val nodeFreeVars = nodeKeys.union(freeVars).intersect(availableVars)
        remainingSumTerms.partition(t =>
          // candidate term must be covered by available vars
          t.isCovered(availableVars) &&
            // delay until either root is reached or
            // all term's variables are marginalised at once
            (tree.isRoot || t.getInputVariables.intersect(nodeFreeVars).isEmpty)
        )
      }
      // Find where conditions at current node. Push conditions down in the tree
      val (nodeWhTerms, restWhTerms) =
        remainingWhTerms.partition(_.isCovered(availableVars))

      // Merge where conditions and sum functions
      val nodeTerms = nodeSumTerms ++ nodeWhTerms
      // Terms can introduce new free variables
      val restTerms = restSumTerms ++ restWhTerms

      // Sanity checks
      if (tree.parent.isEmpty) assert(restTerms.isEmpty)

      // Extend available variables with lift variables
      val liftVarList = nodeSumTerms.flatMap(_.getOutputVariables)
      val newAvailableVarList = availableVarList ++ liftVarList
      val newAvailableVars = newAvailableVarList.toSet

      // Rest terms can introduce new free variables
      val restTermVars = restTerms.flatMap(_.getInputVariables).toSet
      val newFreeVars = freeVars.union(restTermVars)
      val nodeFreeVars = nodeKeys.union(newFreeVars).intersect(newAvailableVars)

      // Compute view free variables
      val viewFreeVars = // preserve the order of tree variables
        newAvailableVarList.filter(nodeFreeVars.contains).map(variableMap.apply)

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
