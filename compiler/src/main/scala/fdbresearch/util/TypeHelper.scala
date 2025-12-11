//===----------------------------------------------------------------------===//
//
// Copyright (c) 2010-2017 EPFL DATA Lab (http://data.epfl.ch)
// 
// Modified by FDB Research Group
//
// https://fdbresearch.github.io/
//
//===----------------------------------------------------------------------===//
package fdbresearch.util

import fdbresearch.core._

case class TypeMismatchException(msg: String) extends Exception(msg) 

/* Helper to convert AST types into language-specific string constants */
object TypeHelper {

  // C++-type specific string functions
  object Cpp {

    def typeToString(t: Type): String = t match {
      case TypeByte   => "int8_t"
      case TypeShort  => "int16_t"
      case TypeInt    => "int32_t"
      case TypeLong   => "int64_t"
      case TypeFloat  => "float"
      case TypeDouble => "double"
      case TypeDate   => "DATE_TYPE"
      case TypeChar   => "char"
      case TypeString => "STRING_TYPE"
      case TypeCustom(d, ps) =>
        if (ps.isEmpty) d.name
        else d.name + "<" + ps.map(paramToString).mkString(", ") + ">"
    }

    def typeToPrimitiveType(t: Type): String = t match {
      case TypeByte   => "PrimitiveType::INT8"
      case TypeShort  => "PrimitiveType::INT16"
      case TypeInt    => "PrimitiveType::INT32"
      case TypeLong   => "PrimitiveType::INT64"
      case TypeFloat  => "PrimitiveType::FLOAT"
      case TypeDouble => "PrimitiveType::DOUBLE"
      case TypeDate   => "PrimitiveType::DATE"
      case TypeChar   => "PrimitiveType::CHAR"
      case TypeString => "PrimitiveType::STRING"
      case TypeCustom(d, ps) => sys.error("No matching primitive type")
    }

    private def paramToString(p: GenericParameter): String = p match {
      case ConstParameter(c) => c.toString
      case PrimitiveTypeParameter(tp) => typeToString(tp)
      case PrioritizedParameterList(p, l) =>
        (p.toString :: l.map(paramToString)).mkString(", ")
    }

    def refTypeToString(t: Type): String = t match {
      case TypeByte   => "int8_t"
      case TypeShort  => "int16_t"
      case TypeInt    => "int32_t"
      case TypeLong   => "int64_t"
      case TypeFloat  => "float"
      case TypeDouble => "double"
      case TypeDate   => "DATE_TYPE"
      case TypeChar   => "char"
      case TypeString => "STRING_TYPE&"
      case TypeCustom(_, _) => typeToString(t) + "&"
    }

    def typeToChar(t: Type): String = t match {
      case TypeByte   => "b"
      case TypeShort  => "s"
      case TypeInt    => "I"
      case TypeLong   => "L"
      case TypeFloat  => "f"
      case TypeDouble => "D"
      case TypeDate   => "T"
      case TypeChar   => "c"
      case TypeString => "S"
      case TypeCustom(d, ps) =>
        if (ps.isEmpty) d.name
        else d.name + ps.map(paramToChar).mkString
    }

    private def paramToChar(p: GenericParameter): String = p match {
      case ConstParameter(c) => c.toString
      case PrimitiveTypeParameter(tp) => typeToChar(tp)
      case PrioritizedParameterList(p, l) => 
        (p.toString :: l.map(paramToChar)).mkString
    }

    def zeroOfType(t: Type): String = t match {
      case TypeByte   => "0"
      case TypeShort  => "0"
      case TypeInt    => "0"
      case TypeLong   => "0L"      
      case TypeFloat  => "0.0f"
      case TypeDouble => "0.0"
      case TypeDate   => "0"
      case TypeChar   => "0"
      case TypeString => "\"\""
      case TypeCustom(_, _) => typeToString(t) + "()"
    }

  }

  // C++-type specific string functions
  object Scala {
    
    def typeToString(t: Type) = t match {
      case TypeByte | TypeShort | TypeInt | TypeLong => "Long"
      case TypeFloat | TypeDouble => "Double"
      case TypeDate   => "Long"
      case TypeChar => "Char"
      case TypeString => "String"
      case t: TypeCustom => sys.error("Custom types not supported")
    }

    def typeToChar(t: Type) = t match {
      case TypeByte | TypeShort | TypeInt | TypeLong => 'L'
      case TypeFloat | TypeDouble => 'D'
      case TypeDate => 'T'
      case TypeChar => 'C'
      case TypeString => 'S'
      case t: TypeCustom => sys.error("Custom types not supported")
    }

    def zeroOfType(t: Type) = t match {
      case TypeByte | TypeShort | TypeInt | TypeLong => "0L"
      case TypeFloat | TypeDouble => "0.0"
      case TypeDate => "0"
      case TypeChar => "0"
      case TypeString => "\"\""
      case t: TypeCustom => sys.error("Custom types not supported")
    }
  }

  def fromString(s: String, tp: Type) = tp match {
    case TypeByte | TypeShort | TypeInt | TypeLong => s.trim.replaceAll("(l|L)$", "").toLong
    case TypeFloat | TypeDouble => s.trim.replaceAll("(l|L|f|F)$", "").toDouble
    case TypeChar => { val t = s.replaceAll("^'|'$", ""); assert(t.size == 1); t(0) }
    case TypeString => s.replaceAll("^\"|\"$", "")
    case TypeDate => s.trim.replaceAll("(l|L)$", "").toLong   // dateConv(v.toLong)
    case _ => sys.error("Cannot convert " + s + " into " + tp)
  }

  // Implicit castings allowed by second-stage compiler ('a' can be promoted to 'b'?)
  def cast(a: Type, b: Type): Boolean = 
    try { b == a.resolve(b) } catch { case TypeMismatchException(msg) => false }

}