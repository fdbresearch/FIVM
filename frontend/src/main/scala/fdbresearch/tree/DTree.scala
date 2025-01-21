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

import scala.annotation.tailrec

/**
  * DTree nodes and operations
  *
  * @author Milos Nikolic
  */
trait DTreeNode {
  def name: String
  def tp: Type
  def keys: List[DTreeVariable]
  override def toString: String =
    name + "[" + keys.map(_.name).mkString(", ") + "]"
}
case class DTreeVariable(name: String, tp: Type, var keys: List[DTreeVariable]) extends DTreeNode

case class DTreeRelation(name: String, keys: List[DTreeVariable]) extends DTreeNode {
  val tp: Type = TypeLong
}

object DTree {

  implicit class DTreeImp(tree: Tree[DTreeNode]) {
    def getVariables: List[DTreeVariable] =
      tree.collect { case v: DTreeVariable => v }.toList

    def getRelations: List[DTreeRelation] =
      tree.collect { case r: DTreeRelation => r }.toList

    // Index in pre-order depth-first search
    def variablePreorderIndex: Int =
      tree.parent.map(p =>
        p.variablePreorderIndex +
          tree.leftSiblings.map(_.getVariables.size).sum + 1
      ).getOrElse(0)
  }

  def apply(sql: SQL.System): Tree[DTreeNode] = {

    // Creates list of variables as DTreeNodes for all variables from the query's relations
    val variables = sql.sources.flatMap(s => s.schema.fields.map(v => DTreeVariable(v._1, v._2, Nil)))
    // Convert them to key-value pairs in a Map
    val variableMap = variables.map(v => v.name -> v).toMap
    // Creates tuples in the form (DTreeVariable, (1, -i) where i is the index of the field stored in the DTreeVariable
    val varScores = sql.sources.flatMap(s => s.schema.fields.zipWithIndex.map { case (f, i) => variableMap(f._1) -> (1, -i) })

    // Order variables by the number of relations in which they appear
    // Resolve ties by preserving the order of variables in a relation
    val varOrder = varScores.groupBy(_._1).toList.map { case (k, v) =>
      val (s1, s2) = v.map(_._2).unzip
      k -> (s1.sum, s2.min)
    }
    val sortedVarOrder = varOrder.sortBy(_._2).map(_._1)

    val relations = sql.sources.map(s => DTreeRelation(s.schema.name, s.schema.fields.map(f => variableMap(f._1))))
    val leaves = relations.map(r => new Tree[DTreeNode](r, None, _ => Nil))

    @tailrec
    def mergeTrees(vars: List[DTreeVariable], trees: List[Tree[DTreeNode]]): List[Tree[DTreeNode]] = vars match {
      case hd :: tl =>
        // 1. Find trees that contain variable 'hd'
        val (treesToMerge, treesToKeep) = trees.partition(_.getRelations.exists(_.keys.contains(hd)))
        // 2. Merge those trees
        val newTrees = if (treesToMerge.isEmpty) treesToKeep else {
          val newTree = new Tree[DTreeNode](hd, None, p => {
            treesToMerge.foreach(_.setParent(p)); treesToMerge
          })
          newTree :: treesToKeep
        }
        mergeTrees(tl, newTrees)
      case Nil => trees
    }

    val mergedTrees = mergeTrees(sortedVarOrder, leaves)
    assert(mergedTrees.size == 1)
    val mergedTree = mergedTrees.head

    // compute keys of each node
    mergedTree.mapWithPostChildren {
      case (v: DTreeVariable, children) =>
        val childKeys = children.flatMap(_.node.keys).distinct
        val keys = childKeys.filter(_ != v)
        DTreeVariable(v.name, v.tp, keys)
      case (r, _) => r
    }
  }
}
