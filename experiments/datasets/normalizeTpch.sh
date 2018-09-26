#!/bin/sh
exec scala "$0" "$@"
!#

object LineitemNormalizer {
  
  import scala.io.Source

  def main(args: Array[String]) {
    if (args.size == 0) {
      println("Specify input file")
      System.exit(0)
    }
    val filename = args(0)
    for (line <- Source.fromFile(filename).getLines) {
      val fields = line.split("\\|")
      val normalized = 
        ( 
          fields.take(3) ++ 
          fields.drop(3).map(f => (f.hashCode.toFloat / Int.MaxValue).toString)
        ).mkString("|")
      println(normalized)
    }
  }
}

LineitemNormalizer.main(args)