package fdbresearch.tree

/**
  * Generic tree implementation
  *
  * @author Milos Nikolic
  */
class Tree[A](val node: A, private var _parent: Option[Tree[A]],
              childrenFactory: Tree[A] => List[Tree[A]]) extends Iterable[A] {

  def parent: Option[Tree[A]] = _parent

  val children: List[Tree[A]] = childrenFactory(this)

  def isRoot: Boolean = parent.isEmpty

  val depth: Int = parent.map(_.depth + 1).getOrElse(0)

  override def toString: String = node.toString + "{" + isRoot + "}" + (children match {
    case Nil => ""
    case hd :: Nil => " - " + hd
    case _ => " - { " + children.mkString(", ") + " }"
  })

  def iterator: Iterator[A] = Iterator.single(node) ++ children.flatMap(_.iterator)

  def map[B](f: A => B): Tree[B] = map2(t => f(t.node))

  def mapWithPostChildren[B](f: (A, List[Tree[B]]) => B): Tree[B] =
    map2WithPostChildren { (t, l) => f(t.node, l) }

  def map2[B](f: Tree[A] => B): Tree[B] = {
    def createChildTrees(children: List[Tree[A]])(parent: Tree[B]): List[Tree[B]] =
      children.map(n => new Tree(f(n), Some(parent), createChildTrees(n.children)))

    new Tree(f(this), None, createChildTrees(children))
  }

  def map2WithPostChildren[B](f: (Tree[A], List[Tree[B]]) => B): Tree[B] = {
    def bottomUp(tree: Tree[A]): Tree[B] = {
      val newChildren = tree.children.map(bottomUp)
      new Tree(f(tree, newChildren), None, p => {
        newChildren.foreach(_._parent = Some(p)); newChildren
      })
    }
    bottomUp(this)
  }

//  // More expensive but immutable implementation
//  def map2WithPostChildren2[B](f: (Tree[A], List[Tree[B]]) => B): Tree[B] = {
//    def create(tree: Tree[A]): Option[Tree[B]] => Tree[B] =
//      parent => {
//        val postChildren = tree.children.map(create)
//        new Tree(f(tree, postChildren.map(g => g(None))), parent, p => postChildren.map(g => g(Some(p))))
//      }
//    create(this)(None)
//  }
}
