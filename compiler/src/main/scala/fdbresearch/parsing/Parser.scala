//===----------------------------------------------------------------------===//
//
// Copyright (c) 2010-2017 EPFL DATA Lab (http://data.epfl.ch)
// 
// Modified by FDB Research Group, University of Oxford
//
// https://fdbresearch.github.io/
//
//===----------------------------------------------------------------------===//
package fdbresearch.parsing

import fdbresearch.core._
import scala.util.parsing.combinator.lexical.StdLexical
import scala.util.parsing.combinator.syntactical.StandardTokenParsers

/**
  * Base case-insensitive parser
  *
  * @author TCK, Milos Nikolic
  */

class Parser extends StandardTokenParsers {

  // Add case insensitivity to keywords
  override val lexical: StdLexical = new StdLexical {

    override protected def processIdent(name: String) = {
      val n = name.toLowerCase
      val r = reserved.filter(_.toLowerCase == n)
      if (r.nonEmpty) Keyword(r.head) else super.processIdent(name)
    }

    import scala.util.parsing.input.CharArrayReader.EofCh

    override def whitespace: Parser[Any] = rep(
      whitespaceChar
      | ('/' ~ '/' | '-' ~ '-') ~ rep(chrExcept(EofCh, '\n'))
      | '/' ~ '*' ~ comment
      | '/' ~ '*' ~ failure("unclosed comment")
    )
  }
  
  // Allow matching non-reserved keywords
  import scala.language.implicitConversions

  override implicit def keyword(chars: String): Parser[String] = {
    keywordCache.getOrElseUpdate(
      chars,
      acceptIf(_.chars.toLowerCase == chars.toLowerCase)
      (_ => "Expected '" + chars + "'") ^^ (_.chars)
    )
  }

  lexical.delimiters ++= List("(", ")", ",", ".", ";", "+", "-", ":=", "<", ">", "{", "}")

  // ------------ Literals
  lazy val intLit: Parser[String] =
    opt("+" | "-") ~ numericLit ^^ { case s ~ n => s.getOrElse("") + n }

  lazy val longLit: Parser[String] =
    intLit <~ ("L" | "l")

  lazy val doubleLit: Parser[String] =
    (intLit <~ ".") ~ opt(numericLit) ~ opt(("E" | "e") ~> intLit) ^^ {
      case i ~ d ~ e => i + "." + d.getOrElse("0") + e.map("E" + _).getOrElse("")
    }

  lazy val floatLit: Parser[String] =
    doubleLit <~ ("F"|"f")

  // ------------ Types
  lazy val tpe: Parser[Type] = (
      "char" ~> "(" ~> "1" ~> ")" ^^^ TypeChar
    | ( "string" | ("char" | "varchar") ~> "(" ~> numericLit <~ ")") ^^^ TypeString
    | "char" ^^^ TypeChar
    | "byte" ^^^ TypeByte
    | "short" ^^^ TypeShort
    | "int" ^^^ TypeInt
    | "long" ^^^ TypeLong
    | "float" ^^^ TypeFloat
    | ("decimal" | "double") ^^^ TypeDouble
    | "date" ^^^ TypeDate
    | genericType
    )

  // ------------ Generic type definition
  lazy val genericParam: Parser[GenericParameter] =
    (  intLit ^^ { i => ConstParameter(i.toInt) }
    |  tpe ^^ { t => PrimitiveTypeParameter(t) }
    |  "[" ~> intLit <~ "]" ^^ {
          case p => PrioritizedParameterList(p.toInt, Nil)
       }
    |  "[" ~> (intLit <~ ",") ~ rep1sep(genericParam, ",") <~ "]" ^^ {
          case p ~ l => PrioritizedParameterList(p.toInt, l)
       }
    )

  lazy val genericType: Parser[TypeCustom] =
    acceptIf (x => typeMap.contains(x.chars)) (x => "No such type '" + x.chars + "'") ~
      opt("<" ~> repsep(genericParam, ",") <~ ">") ^^ {
      case i ~ p => TypeCustom(typeMap(i.chars), p.getOrElse(Nil))
    }

  private val typeMap = collection.mutable.Map[String, TypeDefinition]()

  lazy val parameterType: Parser[ParameterType] =
    ( "static" ^^^ StaticParameter
    | "dynamic_sum" ^^^ DynamicSumParameter
    | "dynamic_concat" ^^^ DynamicPrioritizedConcatParameter
    | "dynamic_min" ^^^ DynamicPrioritizedMinParameter
    )

  lazy val typeDef: Parser[TypeDefinition] =
    ("CREATE" ~> opt("DISTRIBUTED") <~ "TYPE") ~ ident ~ ("FROM" ~> sourceIn) ~
      opt("WITH" ~> "PARAMETER" ~> "SCHEMA" ~> "(" ~> repsep(parameterType, ",") <~ ")") <~ ";" ^^ {
        case d ~ i ~ f ~ p =>
          val td = TypeDefinition(i, f, p.getOrElse(Nil), d.isDefined)
          typeMap += ((i, td))
          td
    }

  // ------------ Function name
  lazy val funcName: Parser[String] = ident

  // ------------ Source declaration
  lazy val source: Parser[Source] =
    "CREATE" ~> ("STREAM" | "TABLE") ~ schema ~ ("FROM" ~> sourceIn) ~
      split ~ adaptor ~ opt(partitioning) <~ ";" ^^ {
      case t ~ s ~ i ~ b ~ a ~ p =>
        Source(t == "STREAM", s, i, b, a, p.getOrElse(LocalExp))
    }

  lazy val schema: Parser[Schema] =
    ident ~ ("(" ~> rep1sep(ident ~ tpe, ",") <~ ")") ^^ {
      case n ~ f => Schema(n, f.map { case k ~ t => (k, t) })
    }

  lazy val sourceIn: Parser[SourceFile] =
    "FILE" ~> stringLit ^^ { f => SourceFile(f) }

  lazy val split: Parser[Split] =
    ( "LINE" ~ "DELIMITED" ^^^ SplitLine
    | stringLit <~ "DELIMITED" ^^ { x => SplitSep(x) }
    | "FIXEDWIDTH" ~> numericLit ^^ { x => SplitSize(Integer.parseInt(x)) }
    | "PREFIX" ~> numericLit ^^ { x => SplitPrefix(Integer.parseInt(x)) }
    )

  lazy val adaptor: Parser[Adaptor] =
    ident ~ opt("(" ~> repsep(ident ~ (":=" ~> stringLit), ",") <~ ")") ^^ {
      case n ~ os => Adaptor(n, os.getOrElse(Nil).map { case x ~ y => (x, y) }.toMap)
    }

  lazy val partitioning: Parser[LocalityType] =
    ( "PARTITIONED" ~> "BY" ~> "[" ~> repsep(ident ~ (":" ~> tpe), ",") <~ "]" ^^ {
        ks => DistByKeyExp(ks.map { case n ~ t => (n, t) })
      }
    | "PARTITIONED" ~> "RANDOMLY" ^^^ DistRandomExp
    )

  // TODO Maybe change DTREE to VARIABLEORDER (will need to update all example files)
  lazy val variableOrder: Parser[VariableOrderDefinition] =
    "IMPORT" ~> "DTREE" ~> "FROM" ~> sourceIn <~ ";" ^^ { f => VariableOrderDefinition(f) }
}