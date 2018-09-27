#!/bin/bash

sbt "
;run examples/sql/sum/retailer_ALL.sql examples/dtree/retailer.txt -o experiments/src/generated/sum/retailer/Sum_Retailer_DF-INCR_ALL.m3
;run examples/sql/sum/retailer_INVENTORY.sql examples/dtree/retailer.txt -o experiments/src/generated/sum/retailer/Sum_Retailer_DF-INCR_INVENTORY.m3
;run examples/sql/sum/housing_ALL.sql examples/dtree/housing.txt -o experiments/src/generated/sum/housing/Sum_Housing_DF-INCR_ALL.m3
"