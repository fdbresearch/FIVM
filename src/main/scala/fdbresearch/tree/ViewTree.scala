package fdbresearch.tree

import fdbresearch.core.{M3, SQL, Type, TypeChar}
import fdbresearch.util.Utils

case class View(name: String, tp: Type, freeVars: List[DTreeVariable], link: Tree[DTreeNode], liftFn: List[M3.Expr] = Nil) {
  override def toString: String = name + "[" + freeVars.map(_.name).mkString(", ") + "]" +
    "(Type: " + tp +
    (if (liftFn.nonEmpty) ", Lift: " + liftFn.mkString(" * ") else "") + ")"
}

object ViewTree {
  import DTree._

  implicit class ViewTreeImp(vtree: Tree[View]) {

    def getRelations: List[DTreeRelation] = vtree.node.link.getRelations

    def isStatic(stream: String): Boolean = isStatic(Set(stream))

    def isStatic(streams: Set[String]): Boolean =
      vtree.getRelations.forall(r => !streams.contains(r.name))

    def materialize_?(streams: Set[String]): Boolean =
      // materialize if root or there is affected sibling
      vtree.isRoot ||
        vtree.parent.exists(_.children.exists(c =>
          c != vtree && c.getRelations.map(_.name).toSet.intersect(streams).nonEmpty))

    def materializedViews(streams: Set[String]): List[Tree[View]] =
      vtree.map2 { t => if (t.materialize_?(streams)) Some(t) else None }.flatten.toList
  }

  // DTree => ViewTree
  def apply(dtree: Tree[DTreeNode], freeVars: Set[String], lifts: List[SQL.Expr]): Tree[View] = {
    val dtreeVars = dtree.getVariables
    assert(freeVars.subsetOf(dtreeVars.map(_.name).toSet))

    val variableMap = dtreeVars.map(v => v.name -> v).toMap

    def typeOf(e: SQL.Expr): Type = e match {
      case SQL.Field(n, _) => variableMap(n).tp
      case SQL.Const(_, tp) => tp
      case SQL.Apply(_, tp, _) => tp
      case SQL.Add(l, r) => Type.resolve(typeOf(l), typeOf(r))
      case SQL.Sub(l, r) => Type.resolve(typeOf(l), typeOf(r))
      case SQL.Mul(l, r) => Type.resolve(typeOf(l), typeOf(r))
      case SQL.Div(l, r) => Type.resolve(typeOf(l), typeOf(r))
      case SQL.Mod(l, r) => Type.resolve(typeOf(l), typeOf(r))
      case _ => sys.error("Not supported SUM expression: " + e)
    }

    def sqlToM3(e: SQL.Expr): M3.Expr = e match {
      case SQL.Field(n, _) => M3.Ref(n, variableMap(n).tp)
      case SQL.Const(v, tp) => M3.Const(tp, v)
      case SQL.Apply(f, tp, as) => M3.Apply(f, tp, as.map(sqlToM3))
      case SQL.Add(l, r) => M3.Add(sqlToM3(l), sqlToM3(r))
      case SQL.Sub(l, r) => M3.Add(sqlToM3(l), M3.Mul(M3.Const(TypeChar, "-1"), sqlToM3(r)))
      case SQL.Mul(l, r) => M3.Mul(sqlToM3(l), sqlToM3(r))
      case SQL.Div(l, r) => M3.Apply("div", typeOf(e), List(sqlToM3(l), sqlToM3(r)))
      case SQL.Mod(l, r) => M3.Apply("mod", typeOf(e), List(sqlToM3(l), sqlToM3(r)))
      case _ => sys.error("Not supported SUM expression: " + e)
    }

    // Build view tree from dtree
    dtree.map2WithPostChildren[View] { (tree, children) =>
      // Compute lift functions at current node
      val treeVars = tree.getVariables.toSet
      val childVars = tree.children.flatMap(_.getVariables).toSet
      val nodeLifts = lifts.filter(l => !l.isCovered(childVars) && l.isCovered(treeVars)).map(sqlToM3)

      // Lifts can introduce new free variables
      val otherLifts = lifts.filter(!_.isCovered(treeVars))
      val otherLiftVars = otherLifts.flatMap(_.getVariables).toSet
      val extraFreeVars = otherLiftVars.intersect(treeVars.map(_.name))
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
        else (children.map(_.node.tp) ++ nodeLifts.map(_.tp)).reduce(Type.resolve)

      View(viewName, viewType, viewFreeVars, tree, nodeLifts)
    }
  }
}
