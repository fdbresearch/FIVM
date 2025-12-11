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
trait VariableOrderNode {
  def name: String
  def tp: Type
  override def toString: String = name
}
case class VariableOrderVar(name: String, tp: Type) extends VariableOrderNode

case class VariableOrderRelation(name: String, keys: List[VariableOrderVar]) extends VariableOrderNode {
  val tp: Type = TypeLong
  override def toString: String = {
    val keysStr = keys.map(_.name).mkString(", ")
    name + "[" + keysStr + "]"
  }
}

object VariableOrder {

  implicit class VariableOrderImp(tree: Tree[VariableOrderNode]) {
    def getVariables: List[VariableOrderVar] =
      tree.collect { case v: VariableOrderVar => v }.toList

    def getRelations: List[VariableOrderRelation] =
      tree.collect { case r: VariableOrderRelation => r }.toList

    def getKeys: List[VariableOrderVar] = {
      tree.node match {
        case v: VariableOrderVar => (tree.children.flatMap(_.getKeys).toSet - v).toList
        case r: VariableOrderRelation => r.keys
      }
    }

    // Index in pre-order depth-first search
    def variablePreorderIndex: Int =
      tree.parent.map(p =>
        p.variablePreorderIndex +
          tree.leftSiblings.map(_.getVariables.size).sum + 1
      ).getOrElse(0)
  }

  def apply(sql: SQL.System): Tree[VariableOrderNode] = {

    // Creates list of variables as DTreeNodes for all variables from the query's relations
    val variables = sql.sources.flatMap(s => s.schema.fields.map(v => VariableOrderVar(v._1, v._2)))
    // Convert them to key-value pairs in a Map
    val variableMap = variables.map(v => v.name -> v).toMap
    // Creates tuples in the form (VarOrderVariable, (1, -i) where i is the index of the field stored in the VarOrderVariable
    val varScores = sql.sources.flatMap(s => s.schema.fields.zipWithIndex.map { case (f, i) => variableMap(f._1) -> (1, -i) })

    // Order variables by the number of relations in which they appear
    // Resolve ties by preserving the order of variables in a relation
    val varOrder = varScores.groupBy(_._1).toList.map { case (k, v) =>
      val (s1, s2) = v.map(_._2).unzip
      k -> (s1.sum, s2.min)
    }
    val sortedVarOrder = varOrder.sortBy(_._2).map(_._1)

    val relations = sql.sources.map(s => VariableOrderRelation(s.schema.name, s.schema.fields.map(f => variableMap(f._1))))
    val leaves = relations.map(r => new Tree[VariableOrderNode](r, None, _ => Nil))

    @tailrec
    def mergeTrees(vars: List[VariableOrderVar], trees: List[Tree[VariableOrderNode]]): List[Tree[VariableOrderNode]] = vars match {
      case hd :: tl =>
        // 1. Find trees that contain variable 'hd'
        val (treesToMerge, treesToKeep) = trees.partition(_.getRelations.exists(_.keys.contains(hd)))
        // 2. Merge those trees
        val newTrees = if (treesToMerge.isEmpty) treesToKeep else {
          val newTree = new Tree[VariableOrderNode](hd, None, p => {
            treesToMerge.foreach(_.setParent(p)); treesToMerge
          })
          newTree :: treesToKeep
        }
        mergeTrees(tl, newTrees)
      case Nil => trees
    }

    val mergedTrees = mergeTrees(sortedVarOrder, leaves)
    assert(mergedTrees.size == 1)
    mergedTrees.head
  }
}
