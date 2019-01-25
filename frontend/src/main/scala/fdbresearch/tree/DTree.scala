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
case class DTreeVariable(name: String, tp: Type, keys: List[DTreeVariable]) extends DTreeNode

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
}
