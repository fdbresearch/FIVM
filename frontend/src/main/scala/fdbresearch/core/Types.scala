package fdbresearch.core

/**
  * Basic data types supported by F-IVM and M3
  *
  */
trait Type {
  def resolve(b: Type): Type
}

case object TypeByte extends Type {
  def resolve(b: Type): Type = b match {
    case TypeByte | TypeShort | TypeInt | TypeLong | TypeFloat | TypeDouble => b
    case _ => throw TypeMismatchException("Type mismatch (" + this + ", " + b + ")")
  }

  override def toString = "char"
}

case object TypeShort extends Type {
  def resolve(b: Type): Type = b match {
    case TypeByte | TypeShort => this
    case TypeInt | TypeLong | TypeFloat | TypeDouble => b
    case _ => throw TypeMismatchException("Type mismatch (" + this + ", " + b + ")")
  }

  override def toString = "short"
}

case object TypeInt extends Type {
  def resolve(b: Type): Type = b match {
    case TypeByte | TypeShort | TypeInt | TypeDate => this
    case TypeLong | TypeFloat | TypeDouble => b
    case _ => throw TypeMismatchException("Type mismatch (" + this + ", " + b + ")")
  }

  override def toString = "int"
}

case object TypeLong extends Type {
  def resolve(b: Type): Type = b match {
    case TypeByte | TypeShort | TypeInt | TypeLong | TypeDate => this
    case TypeFloat | TypeDouble => b
    case _ => throw TypeMismatchException("Type mismatch (" + this + ", " + b + ")")
  }

  override def toString = "long"
}

case object TypeFloat extends Type {
  def resolve(b: Type): Type = b match {
    case TypeByte | TypeShort | TypeInt | TypeLong | TypeFloat => this
    case TypeDouble => b
    case _ => throw TypeMismatchException("Type mismatch (" + this + ", " + b + ")")
  }

  override def toString = "float"
}

case object TypeDouble extends Type {
  def resolve(b: Type): Type = b match {
    case TypeByte | TypeShort | TypeInt | TypeLong | TypeFloat | TypeDouble => this
    case _ => throw TypeMismatchException("Type mismatch (" + this + ", " + b + ")")
  }

  override def toString = "double"
}

case object TypeDate extends Type {
  def resolve(b: Type): Type = b match {
    case TypeDate => this
    case TypeInt | TypeLong => b // no other integer types allowed
    case _ => throw TypeMismatchException("Type mismatch (" + this + ", " + b + ")")
  }

  override def toString = "date"
}

case object TypeChar extends Type {
  def resolve(b: Type): Type = b match {
    case TypeChar => this
    case _ => throw TypeMismatchException("Type mismatch (" + this + ", " + b + ")")
  }

  override def toString = "char"
}

case object TypeString extends Type {
  def resolve(b: Type): Type = b match {
    case TypeString => this
    case _ => throw TypeMismatchException("Type mismatch (" + this + ", " + b + ")")
  }

  override def toString = "string"
}

sealed trait GenericParameter
case class ConstParameter(c: Integer) extends GenericParameter {
  override def toString = c.toString
}
case class PrimitiveTypeParameter(tp: Type) extends GenericParameter {
  override def toString = tp.toString
}
case class PrioritizedParameterList(priority: Integer,
                                    params: List[GenericParameter]) extends GenericParameter {
  override def toString = "[" + (priority :: params).mkString(", ") + "]"
}

case class TypeCustom(typeDef: TypeDefinition,
                      params: List[GenericParameter]) extends Type {

  assert(params.size == typeDef.schema.size,
    "Wrong number of generic parameters")

  private def resolveParam(a: GenericParameter,
                           b: GenericParameter,
                           tp: ParameterType): GenericParameter =
    (tp, a, b) match {
      case (StaticParameter, p1, p2) if p1 == p2 => p1
      case (DynamicSumParameter,
            ConstParameter(c1),
            ConstParameter(c2)) => ConstParameter(c1 + c2)
      case (DynamicPrioritizedConcatParameter,
            PrioritizedParameterList(o1, p1),
            PrioritizedParameterList(o2, p2)) if o1 < o2 => PrioritizedParameterList(o1, p1 ++ p2)
      case (DynamicPrioritizedMinParameter,
            ConstParameter(o1),
            ConstParameter(o2)) if o1 < o2 => a
      case (DynamicPrioritizedMinParameter,
            PrioritizedParameterList(o1, _),
            PrioritizedParameterList(o2, _)) if o1 < o2 => a
      case _ =>
        throw TypeMismatchException("Parameter type mismatch (" + a + ", " + b + ")")
    }

  def resolve(b: Type): Type = b match {
    case TypeByte | TypeShort | TypeInt | TypeLong | TypeFloat | TypeDouble => this
    case b: TypeCustom if typeDef == b.typeDef =>
      TypeCustom(typeDef, (params, b.params, typeDef.schema).zipped.map(resolveParam))
    case _ => throw TypeMismatchException("Type mismatch (" + this + ", " + b + ")")
  }

  override def toString =
    if (params.isEmpty) typeDef.name
    else typeDef.name + "<" + params.mkString(", ") + ">"
}

object Type {
  def resolve(tp1: Type, tp2: Type): Type =
    if (tp1 == null || tp2 == null) null else
    try { tp1.resolve(tp2) }
    catch { case _: Throwable =>
      try { tp2.resolve(tp1) }
      catch { case _: Throwable =>
        sys.error("Problem resolving types: " + tp1 + " and " + tp2)
      }
    }

  def isDistributed(tp: Type): Boolean = tp match {
    case c: TypeCustom => c.typeDef.isDistributed
    case _ => false
  }

  def isNumerical(tp: Type): Boolean = isInteger(tp) || isFloat(tp)

  def isInteger(tp: Type): Boolean = tp match {
    case TypeByte | TypeShort | TypeInt | TypeLong => true
    case _ => false
  }

  def isFloat(tp: Type): Boolean = tp match {
    case TypeFloat | TypeDouble => true
    case _ => false
  }
}

final case class TypeMismatchException(private val msg: String = "",
                                       private val cause: Throwable = None.orNull)
  extends Exception(msg, cause)

