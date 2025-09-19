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

CREATE STREAM R5 (D int, FF int)
  FROM FILE 'R5.csv' LINE DELIMITED CSV;

CREATE STREAM R6 (D int, G int)
  FROM FILE 'R6.csv' LINE DELIMITED CSV;

CREATE STREAM R7 (E int, H int)
  FROM FILE 'R7.csv' LINE DELIMITED CSV;  

CREATE STREAM R8 (E int, I int)
  FROM FILE 'R8.csv' LINE DELIMITED CSV;

CREATE STREAM R9 (E int, J int)
  FROM FILE 'R9.csv' LINE DELIMITED CSV;  

--------------------- MAPS ----------------------

DECLARE MAP V_R1(long)[][A: int, B: int] :=
  R1(A, B);

DECLARE MAP V_R2(long)[][A: int, C: int] :=
  R2(A, C);
  
DECLARE MAP V_R3(long)[][C: int, D: int] :=
  R3(C, D);

DECLARE MAP V_R4(long)[][C: int, E: int] :=
  R4(C, E);

DECLARE MAP V_R5(long)[][D: int, FF: int] :=
  R5(D, FF);

DECLARE MAP V_R6(long)[][D: int, G: int] :=
  R6(D, G);

DECLARE MAP V_R7(long)[][E: int, H: int] :=
  R7(E, H);

DECLARE MAP V_R8(long)[][E: int, I: int] :=
  R8(E, I);

DECLARE MAP V_R9(long)[][E: int, J: int] :=
  R9(E, J);

DECLARE MAP V_B(long)[][A: int] :=
  AggSum([A], V_R1(long)[][A, B]<Local>);

DECLARE MAP V_F(long)[][D: int] :=
  AggSum([D], V_R5(long)[][D, F]<Local>);

DECLARE MAP V_G(long)[][D: int] :=
  AggSum([D], V_R6(long)[][D, G]<Local>);

DECLARE MAP V_H(long)[][E: int] :=
  AggSum([E], V_R7(long)[][E, H]<Local>);

DECLARE MAP V_I(long)[][E: int] :=
  AggSum([E], V_R8(long)[][E, F]<Local>);

DECLARE MAP V_J(long)[][E: int] :=
  AggSum([E], V_R9(long)[][E, J]<Local>);

DECLARE MAP V_D(long)[][C: int] :=
  AggSum([C], V_R3(long)[][C, D]<Local> * V_F(long)[][D]<Local> * V_G(long)[][D]<Local>);

DECLARE MAP V_HIJ(long)[][E: int] :=
  V_H(long)[][E]<Local> + V_I(long)[][E]<Local> + V_J(long)[][E]<Local>;

DECLARE MAP V_E(long)[][C: int] :=
  AggSum([C], V_HIJ(long)[][E]<Local> * V_R4(long)[][C, E]<Local>);

DECLARE MAP V_DE(long)[][C: int] :=
  V_D(long)[][C]<Local> + V_E(long)[][C]<Local>;

DECLARE MAP V_C(long)[][A: int] :=
  AggSum([A], V_R2(long)[][A, C]<Local> * V_DE(long)[][C]<Local>);

DECLARE MAP QUERY3(long)[][A: int] :=
  V_B(long)[][A]<Local> * V_C(long)[][A]<Local>;

DECLARE MAP TMP_V_B(long)[][A: int] :=
  AggSum([A], (DELTA R1)(A, B));

DECLARE MAP TMP_V_C(long)[][A: int] :=
  AggSum([A], (DELTA R2)(A, C) * V_DE(long)[][C]<Local>);

DECLARE MAP TMP_V_D(long)[][C: int] :=
  AggSum([C], (DELTA R3)(C, D) * V_F(long)[][D]<Local> * V_G(long)[][D]<Local>);

DECLARE MAP TMP_V_E(long)[][C: int] :=
  AggSum([C], (DELTA R4)(C, E) * V_HIJ(long)[][E]<Local>);

DECLARE MAP TMP_V_F(long)[][D: int] :=
  AggSum([D], (DELTA R5)(D, F));

DECLARE MAP TMP_V_G(long)[][D: int] :=
  AggSum([D], (DELTA R6)(D, G));

DECLARE MAP TMP_V_H(long)[][E: int] :=
  AggSum([E], (DELTA R7)(E, H));

DECLARE MAP TMP_V_I(long)[][E: int] :=
  AggSum([E], (DELTA R8)(E, F));

DECLARE MAP TMP_V_J(long)[][E: int] :=
  AggSum([E], (DELTA R9)(E, J));

-------------------- QUERIES --------------------
DECLARE QUERY QUERY3 := QUERY3(long)[][A]<Local>;

------------------- TRIGGERS --------------------
ON BATCH UPDATE OF R1 {

  V_R1(long)[][A, B]<Local> += (DELTA R1)(A, B);

  TMP_V_B(long)[][A]<Local> := AggSum([A], (DELTA R1)(A, B));

  V_B(long)[][A]<Local> += TMP_V_B(long)[][A]<Local>;
  
  QUERY3(long)[][A]<Local> += TMP_V_B(long)[][A]<Local> * V_C(long)[][A]<Local>;

}

ON BATCH UPDATE OF R2 {

  V_R2(long)[][A, C]<Local> += (DELTA R2)(A, C);

  TMP_V_C(long)[][A]<Local> := AggSum([A], (DELTA R2)(A, C) * V_DE(long)[][C]<Local>);

  V_C(long)[][A]<Local> += TMP_V_C(long)[][A]<Local>;
  
  QUERY3(long)[][A]<Local> += TMP_V_C(long)[][A]<Local> * V_B(long)[][A]<Local>;

}

ON BATCH UPDATE OF R3 {

  V_R3(long)[][C, D]<Local> += (DELTA R3)(C, D);

  TMP_V_D(long)[][C]<Local> := AggSum([C], (DELTA R3)(C, D) * V_F(long)[][D]<Local> * V_G(long)[][D]<Local>);

  V_D(long)[][C]<Local> += TMP_V_D(long)[][C]<Local>;

  V_DE(long)[][C]<Local> += TMP_V_D(long)[][C]<Local>;

  TMP_V_C(long)[][A]<Local> := AggSum([A], TMP_V_D(long)[][C]<Local> * V_R2(long)[][A, C]<Local>);

  V_C(long)[][A]<Local> += TMP_V_C(long)[][A]<Local>;

  QUERY3(long)[][A]<Local> += TMP_V_C(long)[][A]<Local> * V_B(long)[][A]<Local>;

}

ON BATCH UPDATE OF R4 {

  V_R4(long)[][C, E]<Local> += (DELTA R4)(C, E);

  TMP_V_E(long)[][C]<Local> := AggSum([C], (DELTA R4)(C, E) * V_HIJ(long)[][E]<Local>);

  V_E(long)[][C]<Local> += TMP_V_E(long)[][C]<Local>;

  V_DE(long)[][C]<Local> += TMP_V_E(long)[][C]<Local>;

  TMP_V_C(long)[][A]<Local> := AggSum([A], TMP_V_E(long)[][C]<Local> * V_R2(long)[][A, C]<Local>);

  V_C(long)[][A]<Local> += TMP_V_C(long)[][A]<Local>;

  QUERY3(long)[][A]<Local> += TMP_V_C(long)[][A]<Local> * V_B(long)[][A]<Local>;

}

ON BATCH UPDATE OF R5 {

  V_R5(long)[][D, FF]<Local> += (DELTA R5)(D, FF);

  TMP_V_F(long)[][D]<Local> := AggSum([D], (DELTA R5)(D, FF));

  V_F(long)[][D]<Local> += TMP_V_F(long)[][D]<Local>;

  TMP_V_D(long)[][C]<Local> := AggSum([C], TMP_V_F(long)[][D]<Local> * V_G(long)[][D]<Local> * V_R3(long)[][C, D]<Local>);

  V_D(long)[][C]<Local> += TMP_V_D(long)[][C]<Local>;

  V_DE(long)[][C]<Local> += TMP_V_D(long)[][C]<Local>;

  TMP_V_C(long)[][A]<Local> := AggSum([A], TMP_V_D(long)[][C]<Local> * V_R2(long)[][A, C]<Local>);
   
  V_C(long)[][A]<Local> += TMP_V_C(long)[][A]<Local>;

  QUERY3(long)[][A]<Local> += TMP_V_C(long)[][A]<Local> * V_B(long)[][A]<Local>;

}

ON BATCH UPDATE OF R6 {

  V_R6(long)[][D, G]<Local> += (DELTA R6)(D, G);

  TMP_V_G(long)[][D]<Local> := AggSum([D], (DELTA R6)(D, G));

  V_G(long)[][D]<Local> += TMP_V_G(long)[][D]<Local>;

  TMP_V_D(long)[][C]<Local> := AggSum([C], TMP_V_G(long)[][D]<Local> * V_F(long)[][D]<Local> * V_R3(long)[][C, D]<Local>);

  V_D(long)[][C]<Local> += TMP_V_D(long)[][C]<Local>;

  V_DE(long)[][C]<Local> += TMP_V_D(long)[][C]<Local>;

  TMP_V_C(long)[][A]<Local> := AggSum([A], TMP_V_D(long)[][C]<Local> * V_R2(long)[][A, C]<Local>);
   
  V_C(long)[][A]<Local> += TMP_V_C(long)[][A]<Local>;

  QUERY3(long)[][A]<Local> += TMP_V_C(long)[][A]<Local> * V_B(long)[][A]<Local>;

}

ON BATCH UPDATE OF R7 {

  V_R7(long)[][E, H]<Local> += (DELTA R7)(E, H);

  TMP_V_H(long)[][E]<Local> := AggSum([E], (DELTA R7)(E, H));

  V_H(long)[][E]<Local> += TMP_V_H(long)[][E]<Local>;

  V_HIJ(long)[][E]<Local> += TMP_V_H(long)[][E]<Local>;

  TMP_V_E(long)[][C]<Local> := AggSum([C], TMP_V_H(long)[][E]<Local> * V_R4(long)[][C, E]<Local>);

  V_E(long)[][C]<Local> += TMP_V_E(long)[][C]<Local>;

  V_DE(long)[][C]<Local> += TMP_V_E(long)[][C]<Local>;

  TMP_V_C(long)[][A]<Local> := AggSum([A], TMP_V_E(long)[][C]<Local> * V_R2(long)[][A, C]<Local>);
   
  V_C(long)[][A]<Local> += TMP_V_C(long)[][A]<Local>;

  QUERY3(long)[][A]<Local> += TMP_V_C(long)[][A]<Local> * V_B(long)[][A]<Local>;

}

ON BATCH UPDATE OF R8 {

  V_R8(long)[][E, I]<Local> += (DELTA R8)(E, I);

  TMP_V_I(long)[][E]<Local> := AggSum([E], (DELTA R8)(E, I));

  V_I(long)[][E]<Local> += TMP_V_I(long)[][E]<Local>;

  V_HIJ(long)[][E]<Local> += TMP_V_I(long)[][E]<Local>;

  TMP_V_E(long)[][C]<Local> := AggSum([C], TMP_V_I(long)[][E]<Local> * V_R4(long)[][C, E]<Local>);

  V_E(long)[][C]<Local> += TMP_V_E(long)[][C]<Local>;

  V_DE(long)[][C]<Local> += TMP_V_E(long)[][C]<Local>;

  TMP_V_C(long)[][A]<Local> := AggSum([A], TMP_V_E(long)[][C]<Local> * V_R2(long)[][A, C]<Local>);
   
  V_C(long)[][A]<Local> += TMP_V_C(long)[][A]<Local>;

  QUERY3(long)[][A]<Local> += TMP_V_C(long)[][A]<Local> * V_B(long)[][A]<Local>;

}

ON BATCH UPDATE OF R9 {

  V_R9(long)[][E, J]<Local> += (DELTA R9)(E, J);

  TMP_V_J(long)[][E]<Local> := AggSum([E], (DELTA R9)(E, J));

  V_J(long)[][E]<Local> += TMP_V_J(long)[][E]<Local>;

  V_HIJ(long)[][E]<Local> += TMP_V_J(long)[][E]<Local>;

  TMP_V_E(long)[][C]<Local> := AggSum([C], TMP_V_J(long)[][E]<Local> * V_R4(long)[][C, E]<Local>);

  V_E(long)[][C]<Local> += TMP_V_E(long)[][C]<Local>;

  V_DE(long)[][C]<Local> += TMP_V_E(long)[][C]<Local>;

  TMP_V_C(long)[][A]<Local> := AggSum([A], TMP_V_E(long)[][C]<Local> * V_R2(long)[][A, C]<Local>);
   
  V_C(long)[][A]<Local> += TMP_V_C(long)[][A]<Local>;

  QUERY3(long)[][A]<Local> += TMP_V_C(long)[][A]<Local> * V_B(long)[][A]<Local>;

}

ON SYSTEM READY {
  
}
