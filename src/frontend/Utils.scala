package fivm.lib

/**
 * Useful functions and variables that are shared across different compiler parts.
 */
object Utils {

  // Fresh variables name provider
  private val counter = scala.collection.mutable.HashMap[String, Int]()  
  
  def fresh(name: String = "x") = {
    val c = counter.getOrElse(name, 0) + 1
    counter.put(name, c)
    name + c 
  }

  def freshClear() = counter.clear

  // Indent text by n*2 spaces (and trim trailing space)
  def ind(s: String, n: Int = 1) = {
    val i = "  " * n
    i + s.replaceAll("\n? *$", "").replaceAll("\n", "\n" + i) 
  }

  def tup(vs: List[String]) = 
    if (vs.size > 1) "(" + vs.mkString(", ") + ")" else vs.mkString(", ")

  def block(s: String) = 
    if (s == null || s.trim() == "") s.trim() else "\n" + s + "\n"

  def stringIf(flag: Boolean, t: => String, e: => String = "") = if (flag) t else e

  def delta(name: String) = "DELTA_" + name

}
