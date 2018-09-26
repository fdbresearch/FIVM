package fdbresearch.core

/**
  * Basic data types supported by F-IVM and M3
  *
  */
trait Type {
  def resolve(b: Type): Type
}

case object TypeChar extends Type {
  def resolve(b: Type): Type = b match {
    case TypeChar | TypeShort | TypeInt | TypeLong | TypeFloat | TypeDouble => b
    case _ => throw TypeMismatchException("Type mismatch (" + this + ", " + b + ")")
  }

  override def toString = "char"
}

case object TypeShort extends Type {
  def resolve(b: Type): Type = b match {
    case TypeChar | TypeShort => this
    case TypeInt | TypeLong | TypeFloat | TypeDouble => b
    case _ => throw TypeMismatchException("Type mismatch (" + this + ", " + b + ")")
  }

  override def toString = "short"
}

case object TypeInt extends Type {
  def resolve(b: Type): Type = b match {
    case TypeChar | TypeShort | TypeInt | TypeDate => this
    case TypeLong | TypeFloat | TypeDouble => b
    case _ => throw TypeMismatchException("Type mismatch (" + this + ", " + b + ")")
  }

  override def toString = "int"
}

case object TypeLong extends Type {
  def resolve(b: Type): Type = b match {
    case TypeChar | TypeShort | TypeInt | TypeLong | TypeDate => this
    case TypeFloat | TypeDouble => b
    case _ => throw TypeMismatchException("Type mismatch (" + this + ", " + b + ")")
  }

  override def toString = "long"
}

case object TypeFloat extends Type {
  def resolve(b: Type): Type = b match {
    case TypeChar | TypeShort | TypeInt | TypeLong | TypeFloat => this
    case TypeDouble => b
    case _ => throw TypeMismatchException("Type mismatch (" + this + ", " + b + ")")
  }

  override def toString = "float"
}

case object TypeDouble extends Type {
  def resolve(b: Type): TypeDouble.type = b match {
    case TypeChar | TypeShort | TypeInt | TypeLong | TypeFloat | TypeDouble => this
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

case object TypeString extends Type {
  def resolve(b: Type): TypeString.type = b match {
    case TypeString => this
    case _ => throw TypeMismatchException("Type mismatch (" + this + ", " + b + ")")
  }

  override def toString = "string"
}

case class TypeCustom(name: String, path: String) extends Type {
  def resolve(b: Type): TypeCustom = b match {
    case TypeChar | TypeShort | TypeInt | TypeLong | TypeFloat | TypeDouble => this
    case b: TypeCustom if this == b => this
    case _ => throw TypeMismatchException("Type mismatch (" + this + ", " + b + ")")
  }

  override def toString = name
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
}

final case class TypeMismatchException(private val msg: String = "",
                                       private val cause: Throwable = None.orNull)
  extends Exception(msg, cause)

