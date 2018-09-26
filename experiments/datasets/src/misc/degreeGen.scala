def degree(n: Int): List[List[Int]] = n match {
    case 1 => List(List(0), List(1), List(2))
    case deg => 
        for {
            list <- degree(n - 1)
            i <- 0 to 2
            if list.sum + i <= 2
        } yield list :+ i
}

def printDegrees(l: List[List[Int]]) = l.foreach(line => println(line.mkString("|")))