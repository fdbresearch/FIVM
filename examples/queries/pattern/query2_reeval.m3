---------------- TYPE DEFINITIONS ---------------


-------------------- SOURCES --------------------
CREATE STREAM R1 (A int, B int)
  FROM FILE 'R1.csv' LINE DELIMITED CSV;

CREATE STREAM R2 (A int, C int)
  FROM FILE 'R2.csv' LINE DELIMITED CSV;

CREATE STREAM R3 (C int, D int)
  FROM FILE 'R3.csv' LINE DELIMITED CSV;

CREATE STREAM R4 (C int, E int)
  FROM FILE 'R4.csv' LINE DELIMITED CSV;

--------------------- MAPS ----------------------

DECLARE MAP V_R1(long)[][A: int, B: int] := 
  R1(A,B);

DECLARE MAP V_R2(long)[][A: int, C: int] := 
  R2(A,C);
  
DECLARE MAP V_R3(long)[][C: int, D: int] := 
  R3(C,D);

DECLARE MAP V_R4(long)[][C: int, E: int] := 
  R4(C,E);

DECLARE MAP V_B(long)[][A: int] :=
  AggSum([A], V_R1(long)[][A, B]<Local>);

DECLARE MAP V_D(long)[][C: int] :=
  AggSum([C], V_R3(long)[][C, D]<Local>);

DECLARE MAP V_E(long)[][C: int] :=
  AggSum([C], V_R4(long)[][C, E]<Local>);

DECLARE MAP V_C(long)[][A: int, C: int] :=
  V_R2(long)[][A, C]<Local> * (V_D(long)[][C]<Local> + V_E(long)[][C]<Local>);

DECLARE MAP V_C1(long)[][A: int] :=
  AggSum([A], V_C(long)[][A, C]<Local>);

DECLARE MAP QUERY2(long)[][A: int] :=
  V_B(long)[][A]<Local> + V_C1(long)[][A]<Local>;


-------------------- QUERIES --------------------
DECLARE QUERY QUERY2 := QUERY2(long)[][A]<Local>;

------------------- TRIGGERS --------------------
ON BATCH UPDATE OF R1 {

  V_R1(long)[][A, B]<Local> += (DELTA R1)(A, B);

  V_B(long)[][A]<Local> := AggSum([A], V_R1(long)[][A, B]<Local>);
  
  QUERY2(long)[][A]<Local> := V_B(long)[][A]<Local> + V_C1(long)[][A]<Local>;

}

ON BATCH UPDATE OF R2 {

  V_R2(long)[][A, C]<Local> += (DELTA R2)(A, C);

  V_C(long)[][A, C]<Local> := V_R2(long)[][A, C]<Local> * (V_D(long)[][C]<Local> + V_E(long)[][C]<Local>);
  
  V_C1(long)[][A]<Local> := AggSum([A], V_C(long)[][A, C]<Local>);

  QUERY2(long)[][A]<Local> := V_B(long)[][A]<Local> + V_C1(long)[][A]<Local>;

}

ON BATCH UPDATE OF R3 {

  V_R3(long)[][C, D]<Local> += (DELTA R3)(C, D);

  V_D(long)[][C]<Local> := AggSum([C], V_R3(long)[][C, D]<Local>);

  V_C(long)[][A, C]<Local> := V_R2(long)[][A, C]<Local> * (V_D(long)[][C]<Local> + V_E(long)[][C]<Local>);
  
  V_C1(long)[][A]<Local> := AggSum([A], V_C(long)[][A, C]<Local>);

  QUERY2(long)[][A]<Local> := V_B(long)[][A]<Local> + V_C1(long)[][A]<Local>;

}

ON BATCH UPDATE OF R4 {

  V_R4(long)[][C, E]<Local> += (DELTA R4)(C, E);

  V_E(long)[][C]<Local> := AggSum([C], V_R4(long)[][C, E]<Local>);

  V_C(long)[][A, C]<Local> := V_R2(long)[][A, C]<Local> * (V_D(long)[][C]<Local> + V_E(long)[][C]<Local>);
  
  V_C1(long)[][A]<Local> := AggSum([A], V_C(long)[][A, C]<Local>);

  QUERY2(long)[][A]<Local> := V_B(long)[][A]<Local> + V_C1(long)[][A]<Local>;

}

ON SYSTEM READY {
  
}
