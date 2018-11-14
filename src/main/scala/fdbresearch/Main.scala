package fdbresearch

import fdbresearch.parsing.{DTreeParser, SQLParser}
import org.slf4j.LoggerFactory

object Main extends App {

  System.setProperty(org.slf4j.impl.SimpleLogger.DEFAULT_LOG_LEVEL_KEY, "INFO")

  val logger = LoggerFactory.getLogger("fdbresearch.fivm")

  case class Config( inputSQL: String = "",
                     inputDTree: String = "",
                     batchUpdates: Boolean = true,
                     outputM3: Option[String] = None,
                     factorizedOutput: Boolean = false)

  val parser = new scopt.OptionParser[Config]("sbt run") {
    override def showUsageOnError = true

    head("F-IVM", "0.1")

    arg[String]("<SQL>").required().action((x, c) =>
      c.copy(inputSQL = x)).text("Input SQL file")

    arg[String]("<DTree>").required().action((x, c) =>
      c.copy(inputDTree = x)).text("Input DTree file")

    opt[String]('o', "output").valueName("<M3 file>").action((x, c) =>
      c.copy(outputM3 = Some(x))).text("Output M3 file")

    opt[Unit]("batch").action((_, c) =>
      c.copy(batchUpdates = true)).text("Generate batch update triggers")

    opt[Unit]("single").action((_, c) =>
      c.copy(batchUpdates = false)).text("Generate single-tuple update triggers")

    opt[Unit]("factorized").action((_, c) =>
      c.copy(factorizedOutput = true)).text("Generate factorized output maps")

    help("help").text("prints this usage text")
  }

  parser.parse(args, Config()) match {
    case Some(config) =>
      val sql = new SQLParser().apply(
        scala.io.Source.fromFile(config.inputSQL).mkString)

      logger.debug("SQL FILE: " + sql.toString)

      val dtree = new DTreeParser().apply(
        scala.io.Source.fromFile(config.inputDTree).mkString)

      logger.debug("DTREE FILE: " + dtree.toString)

      val output = new Driver().compile(
        sql, dtree, config.batchUpdates, config.factorizedOutput
      )
      config.outputM3 match {
        case Some(file) => new java.io.PrintWriter(file) {
          write(output); close()
        }
        case None => println(output)
      }

    case None => // bad arguments, error message displayed
  }
}