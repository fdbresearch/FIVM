package fdbresearch.parsing

import fdbresearch.core.{Tree => _, _}
import fdbresearch.tree._

/**
  * DTree (variable order) parser
  *
  * @author Milos Nikolic
  */
class DTreeParser extends Parser with (String => Tree[DTreeNode]) {

  private class LinkedNode(val node: DTreeNode, val parent: Option[DTreeNode])

  // ------------ Types
  override lazy val tpe: Parser[Type] = (
      "char" ^^^ TypeChar
    | "short" ^^^ TypeShort
    | "int" ^^^ TypeInt
    | "long" ^^^ TypeLong
    | "float" ^^^ TypeFloat
    | "double" ^^^ TypeDouble
    | "date" ^^^ TypeDate
    | "string" ^^^ TypeString
  )

  private val variableMap = collection.mutable.Map[String, Option[DTreeVariable]]().withDefaultValue(None)

  private lazy val variable: Parser[LinkedNode] =
    intLit ~ ident ~ tpe ~ intLit ~ ("{" ~> repsep(intLit, ",") <~ "}") ~ intLit ^^ {
      case id ~ name ~ tp ~ parentId ~ keyIds ~ cache =>
        val parent = variableMap(parentId)
        val keys = keyIds.map(k => variableMap(k).get)
        val v = DTreeVariable(name, tp, keys)
        variableMap += (id -> Some(v), name -> Some(v))
        new LinkedNode(v, parent)
    }

  private lazy val relation: Parser[LinkedNode] =
    ident ~ intLit ~ repsep(ident, ",") ^^ {
      case name ~ parentId ~ vars =>
        val parent = variableMap(parentId)
        val keys = vars.map(k => variableMap(k).get)
        val r = DTreeRelation(name, keys)
        new LinkedNode(r, parent)
    }

  private lazy val tree: Parser[Tree[DTreeNode]] =
    intLit ~ intLit ~ rep1(variable) ~ rep1(relation) ^^ {
      case _ ~ _ ~ vs ~ rs => buildTree(vs ++ rs)
    }

  private def buildTree(nodes: List[LinkedNode]): Tree[DTreeNode] =
    nodes.filter(_.parent.isEmpty) match {
      case root :: Nil =>
        def createChildTrees(parent: Tree[DTreeNode]): List[Tree[DTreeNode]] = {
          val children = nodes.filter(_.parent.contains(parent.node))
          children.map(n => new Tree(n.node, Some(parent), createChildTrees))
        }
        new Tree(root.node, None, createChildTrees)
      case _ => sys.error("No root or multiple roots")
    }

  def apply(str: String): Tree[DTreeNode] = phrase(tree)(new lexical.Scanner(str)) match {
    case Success(x, _) => x
    case e => sys.error(e.toString)
  }
}
