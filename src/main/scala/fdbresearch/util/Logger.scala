package fdbresearch.util

import org.slf4j.LoggerFactory

object Logger {

  System.setProperty(org.slf4j.impl.SimpleLogger.DEFAULT_LOG_LEVEL_KEY, "INFO")

  val instance = LoggerFactory.getLogger("fdbresearch.fivm")
}
