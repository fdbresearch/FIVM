#!/bin/bash

# sbt "
# ;run examples/sql/sum/retailer_ALL.sql examples/dtree/retailer.txt -o experiments/src/queries/sum/retailer/Sum_Retailer_DF-INCR_ALL_GENERATED.m3
# ;run examples/sql/sum/retailer_INVENTORY.sql examples/dtree/retailer.txt -o experiments/src/queries/sum/retailer/Sum_Retailer_DF-INCR_INVENTORY_GENERATED.m3
# ;run examples/sql/sum/housing_ALL.sql examples/dtree/housing.txt -o experiments/src/queries/sum/housing/Sum_Housing_DF-INCR_ALL_GENERATED.m3
# ;run examples/sql/cofactor/retailer_ALL.sql examples/dtree/retailer.txt -o experiments/src/queries/cofactor/retailer/Cofactor_Retailer_DF-INCR_ALL_GENERATED.m3
# ;run examples/sql/cofactor/retailer_INVENTORY.sql examples/dtree/retailer.txt -o experiments/src/queries/cofactor/retailer/Cofactor_Retailer_DF-INCR_INVENTORY_GENERATED.m3
# ;run examples/sql/cofactor/housing_ALL.sql examples/dtree/housing.txt -o experiments/src/queries/cofactor/housing/Cofactor_Housing_DF-INCR_ALL_GENERATED.m3
# ;run examples/sql/relational/retailer_ALL.sql examples/dtree/retailer.txt -o experiments/src/queries/full_join/retailer/Relational_Retailer_DF-INCR_ALL_GENERATED.m3
# ;run examples/sql/relational/retailer_INVENTORY.sql examples/dtree/retailer.txt -o experiments/src/queries/full_join/retailer/Relational_Retailer_DF-INCR_INVENTORY_GENERATED.m3
# ;run examples/sql/relational/housing_ALL.sql examples/dtree/housing.txt -o experiments/src/queries/full_join/housing/Relational_Housing_DF-INCR_ALL_GENERATED.m3
# ;run examples/sql/factorized/retailer_ALL.sql examples/dtree/retailer.txt -o experiments/src/queries/full_join/retailer/Factorized_Retailer_DF-INCR_ALL_GENERATED.m3
# ;run examples/sql/factorized/retailer_INVENTORY.sql examples/dtree/retailer.txt -o experiments/src/queries/full_join/retailer/Factorized_Retailer_DF-INCR_INVENTORY_GENERATED.m3
# ;run examples/sql/factorized/housing_ALL.sql examples/dtree/housing.txt -o experiments/src/queries/full_join/housing/Factorized_Housing_DF-INCR_ALL_GENERATED.m3
# "

sbt "
; run examples/sql/factorized/housing_ALL.sql examples/dtree/housing.txt -o experiments/src/queries/full_join/housing/Factorized_Housing_DF-INCR_ALL_GENERATED.m3
; run examples/sql/factorized/retailer_ALL.sql examples/dtree/retailer.txt -o experiments/src/queries/full_join/retailer/Factorized_Retailer_DF-INCR_ALL_GENERATED.m3
; run examples/sql/factorized/retailer_INVENTORY.sql examples/dtree/retailer.txt -o experiments/src/queries/full_join/retailer/Factorized_Retailer_DF-INCR_INVENTORY_GENERATED.m3
;
; run examples/sql/relational/housing_ALL.sql examples/dtree/housing.txt -o experiments/src/queries/full_join/housing/Relational_Housing_DF-INCR_ALL_GENERATED.m3
; run examples/sql/relational/retailer_ALL.sql examples/dtree/retailer.txt -o experiments/src/queries/full_join/retailer/Relational_Retailer_DF-INCR_ALL_GENERATED.m3
; run examples/sql/relational/retailer_INVENTORY.sql examples/dtree/retailer.txt -o experiments/src/queries/full_join/retailer/Relational_Retailer_DF-INCR_INVENTORY_GENERATED.m3
;
; run examples/sql/cofactor/housing_ALL.sql examples/dtree/housing.txt -o experiments/src/queries/cofactor/housing/Cofactor_Housing_DF-INCR_ALL_GENERATED.m3 
; run examples/sql/cofactor/retailer_ALL.sql examples/dtree/retailer.txt -o experiments/src/queries/cofactor/retailer/Cofactor_Retailer_DF-INCR_ALL_GENERATED.m3 
; run examples/sql/cofactor/retailer_INVENTORY.sql examples/dtree/retailer.txt -o experiments/src/queries/cofactor/retailer/Cofactor_Retailer_DF-INCR_INVENTORY_GENERATED.m3
;
; run examples/sql/tpch/query01_ALL.sql examples/dtree/tpch/query01.txt -o experiments/src/queries/sum/tpch/TPCH1_DF-INCR_ALL_GENERATED.m3
; run examples/sql/tpch/query03_ALL.sql examples/dtree/tpch/query03.txt -o experiments/src/queries/sum/tpch/TPCH3_DF-INCR_ALL_GENERATED.m3 
; run examples/sql/tpch/query06_ALL.sql examples/dtree/tpch/query06.txt -o experiments/src/queries/sum/tpch/TPCH6_DF-INCR_ALL_GENERATED.m3
; run examples/sql/tpch/query10_ALL.sql examples/dtree/tpch/query10.txt -o experiments/src/queries/sum/tpch/TPCH10_DF-INCR_ALL_GENERATED.m3
; run examples/sql/tpch/query12_ALL.sql examples/dtree/tpch/query12.txt -o experiments/src/queries/sum/tpch/TPCH12_DF-INCR_ALL_GENERATED.m3
; run examples/sql/tpch/query14_ALL.sql examples/dtree/tpch/query14.txt -o experiments/src/queries/sum/tpch/TPCH14_DF-INCR_ALL_GENERATED.m3
;
; run examples/sql/factorized/housing_ALL.sql examples/dtree/housing.txt -o experiments/src/queries/full_join/housing/Factorized_Housing_DF-INCR_ALL_GENERATED_SINGLE.m3 --single
; run examples/sql/factorized/retailer_ALL.sql examples/dtree/retailer.txt -o experiments/src/queries/full_join/retailer/Factorized_Retailer_DF-INCR_ALL_GENERATED_SINGLE.m3 --single
; run examples/sql/factorized/retailer_INVENTORY.sql examples/dtree/retailer.txt -o experiments/src/queries/full_join/retailer/Factorized_Retailer_DF-INCR_INVENTORY_GENERATED_SINGLE.m3 --single
;
; run examples/sql/relational/housing_ALL.sql examples/dtree/housing.txt -o experiments/src/queries/full_join/housing/Relational_Housing_DF-INCR_ALL_GENERATED_SINGLE.m3 --single
; run examples/sql/relational/retailer_ALL.sql examples/dtree/retailer.txt -o experiments/src/queries/full_join/retailer/Relational_Retailer_DF-INCR_ALL_GENERATED_SINGLE.m3 --single
; run examples/sql/relational/retailer_INVENTORY.sql examples/dtree/retailer.txt -o experiments/src/queries/full_join/retailer/Relational_Retailer_DF-INCR_INVENTORY_GENERATED_SINGLE.m3 --single
;
; run examples/sql/cofactor/housing_ALL.sql examples/dtree/housing.txt -o experiments/src/queries/cofactor/housing/Cofactor_Housing_DF-INCR_ALL_GENERATED_SINGLE.m3 --single 
; run examples/sql/cofactor/retailer_ALL.sql examples/dtree/retailer.txt -o experiments/src/queries/cofactor/retailer/Cofactor_Retailer_DF-INCR_ALL_GENERATED_SINGLE.m3 --single 
; run examples/sql/cofactor/retailer_INVENTORY.sql examples/dtree/retailer.txt -o experiments/src/queries/cofactor/retailer/Cofactor_Retailer_DF-INCR_INVENTORY_GENERATED_SINGLE.m3 --single
;
; run examples/sql/tpch/query01_ALL.sql examples/dtree/tpch/query01.txt -o experiments/src/queries/sum/tpch/TPCH1_DF-INCR_ALL_GENERATED_SINGLE.m3 --single
; run examples/sql/tpch/query03_ALL.sql examples/dtree/tpch/query03.txt -o experiments/src/queries/sum/tpch/TPCH3_DF-INCR_ALL_GENERATED_SINGLE.m3 --single
; run examples/sql/tpch/query06_ALL.sql examples/dtree/tpch/query06.txt -o experiments/src/queries/sum/tpch/TPCH6_DF-INCR_ALL_GENERATED_SINGLE.m3 --single
; run examples/sql/tpch/query10_ALL.sql examples/dtree/tpch/query10.txt -o experiments/src/queries/sum/tpch/TPCH10_DF-INCR_ALL_GENERATED_SINGLE.m3 --single
; run examples/sql/tpch/query12_ALL.sql examples/dtree/tpch/query12.txt -o experiments/src/queries/sum/tpch/TPCH12_DF-INCR_ALL_GENERATED_SINGLE.m3 --single
; run examples/sql/tpch/query14_ALL.sql examples/dtree/tpch/query14.txt -o experiments/src/queries/sum/tpch/TPCH14_DF-INCR_ALL_GENERATED_SINGLE.m3 --single
"