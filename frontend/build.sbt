name := "FIVM"

version := "1.0"

scalaVersion := "2.12.6"

mainClass in (Compile, run) := Some("fdbresearch.Main")

libraryDependencies ++= Seq(
  "org.scala-lang.modules" %% "scala-parser-combinators" % "1.1.1",
  "com.github.scopt" %% "scopt" % "3.7.1",
  "org.slf4j" % "slf4j-api" % "1.7.25",
  "org.slf4j" % "slf4j-simple" % "1.7.25"
)
