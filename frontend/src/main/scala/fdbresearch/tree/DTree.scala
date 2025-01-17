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

  // Below is where we're going to implement creating a Tree[DTreeNode] direct from sql
  // Basic idea:
  // 1. Using SQL object extract info and create DTreeNodes
  // 2. Assemble them into a tree of LinkedNodes, and then flatten these into a List[LinkedNode] like in DTreeParser
  // 3. Build a Tree[DTreeNode] from the aforementioned list
  private class LinkedNode(val node: DTreeNode, val children: Option[List[LinkedNode]], val relations: Set[String])
  private val variableMap = collection.mutable.Map[String, (DTreeVariable, Set[String])]()
  def apply(sql: SQL.System): Tree[DTreeNode] = {
    // 1. Go through each source and the fields to the Map as Key=fieldName, value=(variableNode, relationsSet)
    def addSchemaToMap(schema: Schema): Unit = {
      schema.fields.foreach(addFieldToMap)
      def addFieldToMap(field: (String, Type)): Unit = {
        val (variableNode, relations) = variableMap.getOrElse(field._1, (DTreeVariable(field._1, field._2, List.empty), Set(schema.name)))
        variableMap.update(field._1, (variableNode, relations + schema.name))
      }
    }
    sql.sources.foreach { src => addSchemaToMap(src.schema)}

    // 2. Create a list of DTreeRelation nodes wrapped in LinkedNodes for each relation
    // This will be the list of subtrees which will be combined into 1 tree of linked LinkedNodes
    val subtrees = sql.sources.map({
      src => {
        val keys: List[DTreeVariable] = src.schema.fields.map(
          field => {
            variableMap(field._1) match {
              case (variable, _) => variable
            }
          }
        )
        val relation = DTreeRelation(src.schema.name, keys)
        new LinkedNode(relation, None, Set(src.schema.name))
      }
    })

    // 3. Create a sorted list of all the key-value pairs in the Map
    val sortedVariableList = variableMap.toList
      .sortBy { case (_, (_, set)) => set.size }
      .map { case (_, value) => value }

    // 4. For each field in the sortedVariableList, create a LinkedNode that joins the
    // appropriate subtrees. This will ultimately result in one root
    @tailrec
    def createLinkedNodes(fields: List[(DTreeVariable, Set[String])], subtrees:List[LinkedNode]):List[LinkedNode] = {
      fields match {
        case (variable, relations) :: tail =>
          // 1. Find subtrees that contain those relations and those that don't
          val (subtreesToMerge, subtreesToKeep) = subtrees.partition(linkedNode => linkedNode.relations.intersect(relations).nonEmpty)
          // 2. Make a new LinkNode to link those subtrees at this variable
          val newLinkedNode = new LinkedNode(variable, Option(subtreesToMerge), relations)
          // 3. Subtrees should now be:
          val updatedSubTrees = subtreesToKeep :+ newLinkedNode
          // 4. Call the method again
          createLinkedNodes(tail, updatedSubTrees)
        case Nil => subtrees
      }
    }
    // TODO: Maybe assert here that creatLinkedNodes ultimately only return a list of length one
    val root:LinkedNode = createLinkedNodes(sortedVariableList, subtrees).head

    // 5. Now we have a tree of linked nodes we can obtain the keys for the variables
    def populateKeys(currentNode: LinkedNode): Set[DTreeVariable] = {
      currentNode.node match {
        case variable: DTreeVariable =>
          val childrenKeys: Set[DTreeVariable] = currentNode.children match {
            case Some(children) => children.flatMap(child => populateKeys(child)).toSet
            case None => Set.empty
          }
          val currentKeys = childrenKeys - variable
          variable.keys = currentKeys.toList
          currentKeys
        case relation: DTreeRelation => relation.keys.toSet
      }
    }
    populateKeys(root)

    // 6. Flatten the 'tree' of linked nodes into a list
    def createLinkedNodeList(currentNode: LinkedNode): List[LinkedNode] = {
      currentNode.children match {
        case Some(list) => list.flatMap(createLinkedNodeList) :+ currentNode
        case None => List(currentNode)
      }
    }
    val linkedNodeList = createLinkedNodeList(root)

    // 7. Build the actual tree from the list of linked nodes
    def createChildTrees(parent: Tree[DTreeNode]): List[Tree[DTreeNode]] = {
      linkedNodeList.filter(_.node.equals(parent.node)).head.children match {
        case Some(list) => list.map(n => new Tree(n.node, Some(parent), createChildTrees))
        case None => List.empty
      }
    }
    new Tree(root.node, None, createChildTrees)
  }
}
