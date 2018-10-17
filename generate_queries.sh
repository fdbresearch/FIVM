#!/bin/bash

sbt "
;run examples/sql/sum/retailer_ALL.sql examples/dtree/retailer.txt -o experiments/src/queries/sum/retailer/Sum_Retailer_DF-INCR_ALL_GENERATED.m3
;run examples/sql/sum/retailer_INVENTORY.sql examples/dtree/retailer.txt -o experiments/src/queries/sum/retailer/Sum_Retailer_DF-INCR_INVENTORY_GENERATED.m3
;run examples/sql/sum/housing_ALL.sql examples/dtree/housing.txt -o experiments/src/queries/sum/housing/Sum_Housing_DF-INCR_ALL_GENERATED.m3
;run examples/sql/cofactor/housing_ALL.sql examples/dtree/housing.txt -o experiments/src/queries/cofactor/housing/Cofactor_Housing_DF-INCR_ALL_GENERATED.m3
"

# ;run examples/sql/cofactor/retailer_ALL.sql examples/dtree/retailer.txt -o experiments/src/queries/cofactor/retailer/Cofactor_Retailer_DF-INCR_ALL_GENERATED.m3
# ;run examples/sql/cofactor/retailer_INVENTORY.sql examples/dtree/retailer.txt -o experiments/src/queries/cofactor/retailer/Cofactor_Retailer_DF-INCR_INVENTORY_GENERATED.m3
