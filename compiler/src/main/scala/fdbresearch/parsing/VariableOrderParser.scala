//===----------------------------------------------------------------------===//
//
// Factorized IVM (F-IVM)
//
// https://fdbresearch.github.io/
//
// Copyright (c) 2018-2019, FDB Research Group, University of Oxford
// 
//===----------------------------------------------------------------------===//
package fdbresearch.parsing

import fdbresearch.core.{Tree => _, _}
import fdbresearch.tree._

/**
  * DTree (variable order) parser
  *
  * @author Milos Nikolic
  */
class VariableOrderParser extends Parser with (String => Tree[VariableOrderNode]) {

  private class LinkedNode(val node: VariableOrderNode, val parent: Option[VariableOrderNode])

  // ------------ Types
  override lazy val tpe: Parser[Type] = (
      "byte" ^^^ TypeByte
    | "short" ^^^ TypeShort
    | "int" ^^^ TypeInt
    | "long" ^^^ TypeLong
    | "float" ^^^ TypeFloat
    | "double" ^^^ TypeDouble
    | "date" ^^^ TypeDate
    | "char" ^^^ TypeChar
    | "string" ^^^ TypeString
  )

  private val variableMap = collection.mutable.Map[String, Option[VariableOrderVar]]().withDefaultValue(None)

  private lazy val variable: Parser[LinkedNode] =
    intLit ~ ident ~ tpe ~ intLit ~ ("{" ~> repsep(intLit, ",") <~ "}") ~ intLit ^^ {
      case id ~ name ~ tp ~ parentId ~ keyIds ~ cache =>
        val parent = variableMap(parentId)
        val keys = keyIds.map(k => variableMap(k).get)
        val v = VariableOrderVar(name, tp)
        variableMap += (id -> Some(v), name -> Some(v))
        new LinkedNode(v, parent)
    }

  private lazy val relation: Parser[LinkedNode] =
    ident ~ intLit ~ repsep(ident, ",") ^^ {
      case name ~ parentId ~ vars =>
        val parent = variableMap(parentId)
        val keys = vars.map(k => variableMap(k).get)
        val r = VariableOrderRelation(name, keys)
        new LinkedNode(r, parent)
    }

  private lazy val tree: Parser[Tree[VariableOrderNode]] =
    intLit ~ intLit ~ rep1(variable) ~ rep1(relation) ^^ {
      case _ ~ _ ~ vs ~ rs => buildTree(vs ++ rs)
    }

  private def buildTree(nodes: List[LinkedNode]): Tree[VariableOrderNode] =
    nodes.filter(_.parent.isEmpty) match {
      case root :: Nil =>
        def createChildTrees(parent: Tree[VariableOrderNode]): List[Tree[VariableOrderNode]] = {
          val children = nodes.filter(_.parent.contains(parent.node))
          children.map(n => new Tree(n.node, Some(parent), createChildTrees))
        }
        new Tree(root.node, None, createChildTrees)
      case rr => sys.error("No root or multiple roots: " + rr.map(_.toString).mkString("\n"))
    }

  def apply(str: String): Tree[VariableOrderNode] = phrase(tree)(new lexical.Scanner(str)) match {
    case Success(x, _) => x
    case e => sys.error(e.toString)
  }
}
