---------------- TYPE DEFINITIONS ---------------


-------------------- SOURCES --------------------
CREATE STREAM R1 (A int, B int)
  FROM FILE 'R1.csv' LINE DELIMITED CSV;

CREATE STREAM R2 (A int, C int)
  FROM FILE 'R2.csv' LINE DELIMITED CSV;

CREATE STREAM R3 (A int, D int)
  FROM FILE 'R3.csv' LINE DELIMITED CSV;

--------------------- MAPS ----------------------

DECLARE MAP V_R1(long)[][A: int, B: int] := 
  R1(A,B);

DECLARE MAP V_R2(long)[][A: int, C: int] := 
  R2(A,C);
  
DECLARE MAP V_R3(long)[][A: int, D: int] := 
  R3(A,D);

DECLARE MAP V_B(long)[][A: int] :=
  AggSum([A], V_R1(long)[][A, B]<Local>);

DECLARE MAP V_C(long)[][A: int] :=
  AggSum([A], V_R2(long)[][A, C]<Local>);

DECLARE MAP V_D(long)[][A: int] :=
  AggSum([A], V_R3(long)[][A, D]<Local>);

DECLARE MAP QUERY1(long)[][A: int] :=
  V_BC(long)[][A]<Local> * V_D(long)[][A]<Local>;

DECLARE MAP TMP_V_B(long)[][A: int] :=
  AggSum([A], (DELTA R1)(A, B));

DECLARE MAP TMP_V_C(long)[][A: int] :=
  AggSum([A], (DELTA R2)(A, C));

DECLARE MAP TMP_V_D(long)[][A: int] :=
  AggSum([A], (DELTA R3)(A, D));

-------------------- QUERIES --------------------
DECLARE QUERY QUERY1 := QUERY1(long)[][A]<Local>;

------------------- TRIGGERS --------------------
ON BATCH UPDATE OF R1 {

  V_R1(long)[][A, B]<Local> += (DELTA R1)(A, B);

  TMP_V_B(long)[][A]<Local> := AggSum([A], (DELTA R1)(A, B));

  V_B(long)[][A]<Local> += TMP_V_B(long)[][A]<Local>;
  
  QUERY1(long)[][A]<Local> += TMP_V_B(long)[][A]<Local> * V_D(long)[][A]<Local>;

}

ON BATCH UPDATE OF R2 {

  V_R2(long)[][A, C]<Local> += (DELTA R2)(A, C);

  TMP_V_C(long)[][A]<Local> := AggSum([A], (DELTA R2)(A, C));

  V_C(long)[][A]<Local> += TMP_V_C(long)[][A]<Local>;
  
  QUERY1(long)[][A]<Local> += TMP_V_C(long)[][A]<Local> * V_D(long)[][A]<Local>;

}

ON BATCH UPDATE OF R3 {

  V_R3(long)[][A, D]<Local> += (DELTA R3)(A, D);

  TMP_V_D(long)[][A]<Local> := AggSum([A], (DELTA R3)(A, D));

  V_D(long)[][A]<Local> += TMP_V_D(long)[][A]<Local>;

  QUERY1(long)[][A]<Local> += TMP_V_D(long)[][A]<Local> * (V_B(long)[][A]<Local> + V_C(long)[][A]<Local>);

}

ON SYSTEM READY {
  
}
