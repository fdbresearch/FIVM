package fdbresearch

import fdbresearch.core.{LocalExp, M3, Source, TypeDefinition}
import fdbresearch.tree.{DTreeRelation, DTreeVariable, Tree, View}

class CodeGenerator( tree: Tree[View],
                     typeDefs: List[TypeDefinition],
                     sources: List[Source],
                     batchUpdates: Boolean,
                     factorizedOutput: Boolean ) {

  import fdbresearch.tree.ViewTree._

  private val events: List[M3.EventTrigger] =
    sources.filter(_.isStream).flatMap(s =>
      if (batchUpdates) List(M3.EventBatchUpdate(s.schema))
      else List(M3.EventInsert(s.schema), M3.EventDelete(s.schema))
    ) :+ M3.EventReady

  private val streams: Set[String] = events.map(_.schema.name).toSet

  private implicit class ViewTreeToM3Imp(tree: Tree[View]) {

    private def isStatic(streams: Set[String]): Boolean =
      tree.getRelations.forall(r => !streams.contains(r.name))

    def isStatic(stream: String): Boolean = isStatic(Set(stream))

    def isStatic: Boolean = isStatic(streams)

    private def isMaterialized(streams: Set[String]): Boolean =
      // materialize if root or there is affected sibling
      factorizedOutput || tree.isRoot ||
          tree.parent.exists(_.children.exists(c =>
            c != tree && c.getRelations.map(_.name).toSet.intersect(streams).nonEmpty))

    def isMaterialized: Boolean = isMaterialized(streams)

    def createMapRef: M3.MapRef =
      M3.MapRef(tree.node.name, tree.node.tp, tree.node.freeVars.map(v => (v.name, v.tp)))

    def createMapDef: M3.MapDef =
      M3.MapDef(
        tree.node.name, tree.node.tp, tree.node.freeVars.map(v => (v.name, v.tp)),
        Optimizer.optimizeExpr(tree.createDefExpr), LocalExp
      )

    def createExpr: M3.Expr =
      if (isMaterialized(streams)) tree.createMapRef else tree.createDefExpr

    def createDefExpr: M3.Expr = {
      val childJoin = tree.node.link.node match {
        case _: DTreeVariable => tree.children match {
          case Nil => sys.error("Variable as a leaf node")
          case cl =>
            (cl.map(_.createExpr) ++ tree.node.liftFn).reduceLeft(M3.Mul)
        }
        case DTreeRelation(name, keys) =>
          M3.MapRefConst(name, keys.map(v => (v.name, v.tp)))
      }

      val marginalizedVars =
        tree.children.flatMap(_.node.freeVars).toSet
            .diff(tree.node.freeVars.toSet)

      if (marginalizedVars.isEmpty) childJoin
      else M3.AggSum(tree.node.freeVars.map(v => (v.name, v.tp)), childJoin)
    }

    def createDeltaExpr(event: M3.EventTrigger): M3.Expr = {
      val childJoin = tree.node.link.node match {
        case _: DTreeVariable =>
          tree.children.filter(!_.isStatic(event.schema.name)) match {
            case hd :: Nil =>
              ( hd.createDeltaExpr(event) ::
                (hd.rightSiblings ++ hd.leftSiblings.reverse).map(_.createExpr) ++
                  tree.node.liftFn).reduceLeft(M3.Mul)
            case _ => sys.error("# of delta paths not 1")
          }

        case DTreeRelation(name, keys) =>
          assert(name == event.schema.name)
          event match {
            case M3.EventBatchUpdate(_) =>
              M3.DeltaMapRefConst(name, keys.map(v => (v.name, v.tp)))
            case _: M3.EventInsert | _: M3.EventDelete =>
              sys.error("Single-tuple updates not supported yet")
            case M3.EventReady =>
              sys.error("No delta trigger for SystemReady")
          }
      }

      val marginalizedVars =
        tree.children.flatMap(_.node.freeVars).toSet
          .diff(tree.node.freeVars.toSet)

      if (marginalizedVars.isEmpty) childJoin
      else M3.AggSum(tree.node.freeVars.map(v => (v.name, v.tp)), childJoin)
    }
  }

  def generateM3: M3.System =
    Optimizer.optimize(
      M3.System(typeDefs, sources, generateMaps, generateQueries, generateTriggers)
    )

  private def generateMaps: List[M3.MapDef] =
    tree.map2(t => if (t.isMaterialized) Some(t.createMapDef) else None).flatten.toList

  private def generateQueries: List[M3.Query] =
    tree.map2(t =>
      if (factorizedOutput || t.isRoot) Some(M3.Query(t.node.name, t.createMapRef))
      else None
    ).flatten.toList

  private def generateTriggers: List[M3.Trigger] = events.map {
    case M3.EventReady =>
      M3.Trigger(M3.EventReady,
        tree.map2(t =>
          if (t.isStatic && t.isMaterialized)
            Some(M3.TriggerStmt(t.createMapRef, t.createDefExpr, M3.OpSet, None))
          else None
        ).flatten.toList
      )
    case e: M3.EventBatchUpdate => generateUpdateTrigger(e)
    case e: M3.EventInsert => generateUpdateTrigger(e)
    case e: M3.EventDelete => generateUpdateTrigger(e)
  }

  private def generateUpdateTrigger(e: M3.EventTrigger): M3.Trigger =
    M3.Trigger(e,
      tree.map2(t =>
        if (!t.isStatic(e.schema.name) && t.isMaterialized)
          Some(M3.TriggerStmt(t.createMapRef, t.createDeltaExpr(e), M3.OpAdd, None))
        else None
      ).flatten.toList
    )
}
