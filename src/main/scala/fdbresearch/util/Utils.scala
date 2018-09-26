package fdbresearch.util

object Utils {

  // Fresh variables name provider
  private val counter = scala.collection.mutable.HashMap[String, Int]()

  def fresh(name: String = "x"): String = {
    val c = counter.getOrElse(name, 0) + 1
    counter.put(name, c)
    name + c
  }

  def freshClear(): Unit = counter.clear

  // Indent text by n*2 spaces (and trim trailing space)
  def ind(s: String, n: Int = 1): String = {
    val i = "  " * n
    i + s.replaceAll("\n? *$", "").replaceAll("\n", "\n" + i)
  }

  def delta(name: String): String = "DELTA_" + name
}
