//===----------------------------------------------------------------------===//
//
// Factorized IVM (F-IVM)
//
// https://fdbresearch.github.io/
//
// Copyright (c) 2018-2019, FDB Research Group, University of Oxford
// 
//===----------------------------------------------------------------------===//
package fdbresearch

import fdbresearch.parsing.{VariableOrderParser, SQLParser}
import fdbresearch.tree.VariableOrder
import fdbresearch.util.Logger

import java.io.File

object Main extends App {

  case class Config(inputSQL: String = "",
                    batchUpdates: Boolean = true,
                    outputM3: Option[String] = None)

  val parser = new scopt.OptionParser[Config]("sbt run") {
    override def showUsageOnError = true

    head("F-IVM", "1.0")

    arg[String]("<SQL file>").required().action((x, c) =>
      c.copy(inputSQL = x)).text("Input SQL file")

    opt[String]('o', "output").valueName("<M3 file>").action((x, c) =>
      c.copy(outputM3 = Some(x))).text("Output M3 file")

    opt[Unit]("batch").action((_, c) =>
      c.copy(batchUpdates = true)).text("Generate batch update triggers")

    opt[Unit]("single").action((_, c) =>
      c.copy(batchUpdates = false)).text("Generate single-tuple update triggers")

    help("help").text("prints this usage text")
  }

  def parseSQL(f: File) = {
    if (!f.exists) {
      sys.error("Input SQL file does not exist: " + f.getAbsolutePath)
    }
    val source = scala.io.Source.fromFile(f)
    try {
      new SQLParser().apply(source.mkString)
    } finally {
      source.close()
    }
  }

  def parseVariableOrder(f: File) = {
    if (!f.exists) {
      sys.error("Input VariableOrder file does not exist: " + f.getAbsoluteFile)
    }
    val source = scala.io.Source.fromFile(f)
    try {
      new VariableOrderParser().apply(source.mkString)
    } finally {
      source.close()
    }
  }

  parser.parse(args, Config()) match {
    case Some(config) =>
      val sqlFile = new File(config.inputSQL)
      val sql = parseSQL(sqlFile)
      Logger.instance.debug("SQL AST: " + sql.toString)

      val variableOrder = if (sql.variableOrder.isDefined) {
        Logger.instance.debug("VariableOrder obtained from file")
        val variableOrderFile = new File(sqlFile.getParentFile.getAbsolutePath, sql.variableOrder.get.file.path)
        parseVariableOrder(variableOrderFile)
      } else {
        Logger.instance.debug("VariableOrder obtained from VariableOrder.apply()")
        VariableOrder.apply(sql)
      }
      Logger.instance.debug("VariableOrder AST: " + variableOrder.toString)

      val output = new Driver().compile(sql, variableOrder, config.batchUpdates)
      config.outputM3 match {
        case Some(file) => new java.io.PrintWriter(file) {
          write(output); close()
        }
        case None => println(output)
      }

    case None => // bad arguments, error message displayed
  }
}