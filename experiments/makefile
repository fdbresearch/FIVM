CC = g++
CFLAGS = -Wall -Wno-unused-variable -std=c++11 -pedantic
BINDIR = bin
DBTOASTER = release/bin/dbtoaster

NUMBER_OF_RUNS ?= 1
BATCH_SIZE ?= 1000
DBTOASTER_FLAGS ?= -F HEURISTICS-DECOMPOSE-OVER-TABLES
FLAGS ?=
SRC_QUERIES_DIR ?= src/queries

####################
# COFACTOR HOUSING
####################
COFACTOR_HOUSING_SOURCE_EVAL = \
	cofactor/housing/Cofactor_Housing_DF-EVAL.hpp \
	cofactor/housing/Cofactor_Housing_PIVOT-EVAL.m3
	# cofactor/housing/Cofactor_Housing_DBT-EVAL.sql
	
COFACTOR_HOUSING_SOURCE_INCR = \
	cofactor/housing/Cofactor_Housing_DF-INCR_ALL.hpp \
	cofactor/housing/Cofactor_Housing_PIVOT-INCR_ALL.m3
	# cofactor/housing/Cofactor_Housing_DBT-INCR_ALL.sql	

COFACTOR_HOUSING_GENERATE_HPP_FROM_SQL_EVAL = $(patsubst %.sql, $(BINDIR)/queries/%.hpp, $(filter %.sql, $(COFACTOR_HOUSING_SOURCE_EVAL)))
COFACTOR_HOUSING_GENERATE_HPP_FROM_SQL_INCR = $(patsubst %.sql, $(BINDIR)/queries/%.hpp, $(filter %.sql, $(COFACTOR_HOUSING_SOURCE_INCR)))
COFACTOR_HOUSING_GENERATE_HPP_FROM_M3_EVAL = $(patsubst %.m3, $(BINDIR)/queries/%.hpp, $(filter %.m3, $(COFACTOR_HOUSING_SOURCE_EVAL)))
COFACTOR_HOUSING_GENERATE_HPP_FROM_M3_INCR = $(patsubst %.m3, $(BINDIR)/queries/%.hpp, $(filter %.m3, $(COFACTOR_HOUSING_SOURCE_INCR)))
COFACTOR_HOUSING_GENERATE_HPP_FROM_HPP_EVAL = $(patsubst %.hpp, $(BINDIR)/queries/%.hpp, $(filter %.hpp, $(COFACTOR_HOUSING_SOURCE_EVAL)))
COFACTOR_HOUSING_GENERATE_HPP_FROM_HPP_INCR = $(patsubst %.hpp, $(BINDIR)/queries/%.hpp, $(filter %.hpp, $(COFACTOR_HOUSING_SOURCE_INCR)))

COFACTOR_HOUSING_GENERATE_HPP_EVAL = $(COFACTOR_HOUSING_GENERATE_HPP_FROM_SQL_EVAL) $(COFACTOR_HOUSING_GENERATE_HPP_FROM_M3_EVAL) $(COFACTOR_HOUSING_GENERATE_HPP_FROM_HPP_EVAL)
COFACTOR_HOUSING_GENERATE_HPP_INCR = $(COFACTOR_HOUSING_GENERATE_HPP_FROM_SQL_INCR) $(COFACTOR_HOUSING_GENERATE_HPP_FROM_M3_INCR) $(COFACTOR_HOUSING_GENERATE_HPP_FROM_HPP_INCR)

COFACTOR_HOUSING_EXE_EVAL = $(COFACTOR_HOUSING_GENERATE_HPP_EVAL:$(BINDIR)/queries/%.hpp=$(BINDIR)/%) 
COFACTOR_HOUSING_EXE_INCR = $(COFACTOR_HOUSING_GENERATE_HPP_INCR:$(BINDIR)/queries/%.hpp=$(BINDIR)/%_$(BATCH_SIZE))

####################
# COFACTOR RETAILER 
####################
COFACTOR_RETAILER_SOURCE_EVAL = \
	cofactor/retailer/Cofactor_Retailer_DF-EVAL.hpp \
	cofactor/retailer/Cofactor_Retailer_PIVOT-EVAL.m3 \
	cofactor/retailer/Cofactor_Retailer_PIVOT-EVAL_2.m3
	# cofactor/retailer/Cofactor_Retailer_DBT-EVAL.sql

COFACTOR_RETAILER_SOURCE_INCR = \
	cofactor/retailer/Cofactor_Retailer_DF-INCR_ALL.hpp \
	cofactor/retailer/Cofactor_Retailer_DF-INCR_INVENTORY.hpp \
	cofactor/retailer/Cofactor_Retailer_PIVOT-INCR_ALL.m3 \
	cofactor/retailer/Cofactor_Retailer_PIVOT-INCR_INVENTORY.m3 \
	cofactor/retailer/Cofactor_Retailer_PIVOT-INCR_INVENTORY_2.m3 \
	cofactor/retailer/Cofactor_Retailer_PIVOT-INCR_INVENTORY_WEATHER.m3 \
	cofactor/retailer/Cofactor_Retailer_PIVOT-INCR_ITEM.m3
	# cofactor/retailer/Cofactor_Retailer_DBT-INCR_ALL.sql \
	# cofactor/retailer/Cofactor_Retailer_DBT-INCR_INVENTORY.sql

COFACTOR_RETAILER_GENERATE_HPP_FROM_SQL_EVAL = $(patsubst %.sql, $(BINDIR)/queries/%.hpp, $(filter %.sql, $(COFACTOR_RETAILER_SOURCE_EVAL)))
COFACTOR_RETAILER_GENERATE_HPP_FROM_SQL_INCR = $(patsubst %.sql, $(BINDIR)/queries/%.hpp, $(filter %.sql, $(COFACTOR_RETAILER_SOURCE_INCR)))
COFACTOR_RETAILER_GENERATE_HPP_FROM_M3_EVAL = $(patsubst %.m3, $(BINDIR)/queries/%.hpp, $(filter %.m3, $(COFACTOR_RETAILER_SOURCE_EVAL)))
COFACTOR_RETAILER_GENERATE_HPP_FROM_M3_INCR = $(patsubst %.m3, $(BINDIR)/queries/%.hpp, $(filter %.m3, $(COFACTOR_RETAILER_SOURCE_INCR)))
COFACTOR_RETAILER_GENERATE_HPP_FROM_HPP_EVAL = $(patsubst %.hpp, $(BINDIR)/queries/%.hpp, $(filter %.hpp, $(COFACTOR_RETAILER_SOURCE_EVAL)))
COFACTOR_RETAILER_GENERATE_HPP_FROM_HPP_INCR = $(patsubst %.hpp, $(BINDIR)/queries/%.hpp, $(filter %.hpp, $(COFACTOR_RETAILER_SOURCE_INCR)))

COFACTOR_RETAILER_GENERATE_HPP_EVAL = $(COFACTOR_RETAILER_GENERATE_HPP_FROM_SQL_EVAL) $(COFACTOR_RETAILER_GENERATE_HPP_FROM_M3_EVAL) $(COFACTOR_RETAILER_GENERATE_HPP_FROM_HPP_EVAL)
COFACTOR_RETAILER_GENERATE_HPP_INCR = $(COFACTOR_RETAILER_GENERATE_HPP_FROM_SQL_INCR) $(COFACTOR_RETAILER_GENERATE_HPP_FROM_M3_INCR) $(COFACTOR_RETAILER_GENERATE_HPP_FROM_HPP_INCR)

COFACTOR_RETAILER_EXE_EVAL = $(COFACTOR_RETAILER_GENERATE_HPP_EVAL:$(BINDIR)/queries/%.hpp=$(BINDIR)/%) 
COFACTOR_RETAILER_EXE_INCR = $(COFACTOR_RETAILER_GENERATE_HPP_INCR:$(BINDIR)/queries/%.hpp=$(BINDIR)/%_$(BATCH_SIZE))

####################
# COFACTOR TWITTER
####################
COFACTOR_TWITTER_SOURCE_EVAL = \
	cofactor/twitter/Cofactor_Triangle_DF-EVAL.hpp \
	cofactor/twitter/Cofactor_Triangle_DBT-EVAL.sql \
	cofactor/twitter/Cofactor_Triangle_PIVOT-EVAL.m3 \
	cofactor/twitter/Cofactor_Loop4_DF-EVAL.hpp \
	cofactor/twitter/Cofactor_Loop4_DBT-EVAL.sql \
	cofactor/twitter/Cofactor_Loop4_PIVOT-EVAL.m3 \
	cofactor/twitter/Cofactor_LW4_DBT-EVAL.sql

COFACTOR_TWITTER_SOURCE_INCR = \
	cofactor/twitter/Cofactor_Triangle_DF-INCR_R.hpp \
	cofactor/twitter/Cofactor_Triangle_DF-INCR_R_S_T.hpp \
	cofactor/twitter/Cofactor_Triangle_DBT-INCR_R.sql \
	cofactor/twitter/Cofactor_Triangle_DBT-INCR_R_S.sql \
	cofactor/twitter/Cofactor_Triangle_DBT-INCR_R_S_T.sql \
	cofactor/twitter/Cofactor_Triangle_PIVOT-INCR_R.m3 \
	cofactor/twitter/Cofactor_Triangle_PIVOT-INCR_R_S.m3 \
	cofactor/twitter/Cofactor_Triangle_PIVOT-INCR_R_S_T.m3 \
	cofactor/twitter/Cofactor_Loop4_DF-INCR_R.hpp \
	cofactor/twitter/Cofactor_Loop4_DF-INCR_R_S_T_U.hpp \
	cofactor/twitter/Cofactor_Loop4_DBT-INCR_R.sql \
	cofactor/twitter/Cofactor_Loop4_DBT-INCR_R_S_T_U.sql \
	cofactor/twitter/Cofactor_Loop4_PIVOT-INCR_R.m3 \
	cofactor/twitter/Cofactor_Loop4_PIVOT-INCR_R_S_T_U.m3 \
	cofactor/twitter/Cofactor_LW4_DBT-INCR_R.sql \
	cofactor/twitter/Cofactor_LW4_DBT-INCR_R_S_T_U.sql

COFACTOR_TWITTER_GENERATE_HPP_FROM_SQL_EVAL = $(patsubst %.sql, $(BINDIR)/queries/%.hpp, $(filter %.sql, $(COFACTOR_TWITTER_SOURCE_EVAL)))
COFACTOR_TWITTER_GENERATE_HPP_FROM_SQL_INCR = $(patsubst %.sql, $(BINDIR)/queries/%.hpp, $(filter %.sql, $(COFACTOR_TWITTER_SOURCE_INCR)))
COFACTOR_TWITTER_GENERATE_HPP_FROM_M3_EVAL = $(patsubst %.m3, $(BINDIR)/queries/%.hpp, $(filter %.m3, $(COFACTOR_TWITTER_SOURCE_EVAL)))
COFACTOR_TWITTER_GENERATE_HPP_FROM_M3_INCR = $(patsubst %.m3, $(BINDIR)/queries/%.hpp, $(filter %.m3, $(COFACTOR_TWITTER_SOURCE_INCR)))
COFACTOR_TWITTER_GENERATE_HPP_FROM_HPP_EVAL = $(patsubst %.hpp, $(BINDIR)/queries/%.hpp, $(filter %.hpp, $(COFACTOR_TWITTER_SOURCE_EVAL)))
COFACTOR_TWITTER_GENERATE_HPP_FROM_HPP_INCR = $(patsubst %.hpp, $(BINDIR)/queries/%.hpp, $(filter %.hpp, $(COFACTOR_TWITTER_SOURCE_INCR)))

COFACTOR_TWITTER_GENERATE_HPP_EVAL = $(COFACTOR_TWITTER_GENERATE_HPP_FROM_SQL_EVAL) $(COFACTOR_TWITTER_GENERATE_HPP_FROM_M3_EVAL) $(COFACTOR_TWITTER_GENERATE_HPP_FROM_HPP_EVAL)
COFACTOR_TWITTER_GENERATE_HPP_INCR = $(COFACTOR_TWITTER_GENERATE_HPP_FROM_SQL_INCR) $(COFACTOR_TWITTER_GENERATE_HPP_FROM_M3_INCR) $(COFACTOR_TWITTER_GENERATE_HPP_FROM_HPP_INCR)

COFACTOR_TWITTER_EXE_EVAL = $(COFACTOR_TWITTER_GENERATE_HPP_EVAL:$(BINDIR)/queries/%.hpp=$(BINDIR)/%) 
COFACTOR_TWITTER_EXE_INCR = $(COFACTOR_TWITTER_GENERATE_HPP_INCR:$(BINDIR)/queries/%.hpp=$(BINDIR)/%_$(BATCH_SIZE))


####################
# COFACTOR TPCH 
####################
COFACTOR_TPCH_SOURCE_EVAL = \

COFACTOR_TPCH_SOURCE_INCR = \
	cofactor/tpch/Cofactor_TPCH9_DF-INCR_ALL_SIMPLIFIED.hpp \
	cofactor/tpch/Cofactor_TPCH9_DBT-INCR_ALL_SIMPLIFIED.sql
	# cofactor/tpch/Cofactor_TPCH9_DBT-INCR_ALL.sql

COFACTOR_TPCH_GENERATE_HPP_FROM_SQL_EVAL = $(patsubst %.sql, $(BINDIR)/queries/%.hpp, $(filter %.sql, $(COFACTOR_TPCH_SOURCE_EVAL)))
COFACTOR_TPCH_GENERATE_HPP_FROM_SQL_INCR = $(patsubst %.sql, $(BINDIR)/queries/%.hpp, $(filter %.sql, $(COFACTOR_TPCH_SOURCE_INCR)))
COFACTOR_TPCH_GENERATE_HPP_FROM_M3_EVAL = $(patsubst %.m3, $(BINDIR)/queries/%.hpp, $(filter %.m3, $(COFACTOR_TPCH_SOURCE_EVAL)))
COFACTOR_TPCH_GENERATE_HPP_FROM_M3_INCR = $(patsubst %.m3, $(BINDIR)/queries/%.hpp, $(filter %.m3, $(COFACTOR_TPCH_SOURCE_INCR)))
COFACTOR_TPCH_GENERATE_HPP_FROM_HPP_EVAL = $(patsubst %.hpp, $(BINDIR)/queries/%.hpp, $(filter %.hpp, $(COFACTOR_TPCH_SOURCE_EVAL)))
COFACTOR_TPCH_GENERATE_HPP_FROM_HPP_INCR = $(patsubst %.hpp, $(BINDIR)/queries/%.hpp, $(filter %.hpp, $(COFACTOR_TPCH_SOURCE_INCR)))

COFACTOR_TPCH_GENERATE_HPP_EVAL = $(COFACTOR_TPCH_GENERATE_HPP_FROM_SQL_EVAL) $(COFACTOR_TPCH_GENERATE_HPP_FROM_M3_EVAL) $(COFACTOR_TPCH_GENERATE_HPP_FROM_HPP_EVAL)
COFACTOR_TPCH_GENERATE_HPP_INCR = $(COFACTOR_TPCH_GENERATE_HPP_FROM_SQL_INCR) $(COFACTOR_TPCH_GENERATE_HPP_FROM_M3_INCR) $(COFACTOR_TPCH_GENERATE_HPP_FROM_HPP_INCR)

COFACTOR_TPCH_EXE_EVAL = $(COFACTOR_TPCH_GENERATE_HPP_EVAL:$(BINDIR)/queries/%.hpp=$(BINDIR)/%) 
COFACTOR_TPCH_EXE_INCR = $(COFACTOR_TPCH_GENERATE_HPP_INCR:$(BINDIR)/queries/%.hpp=$(BINDIR)/%_$(BATCH_SIZE))


####################
# SUM HOUSING
####################
SUM_HOUSING_SOURCE_EVAL = \
	sum/housing/Sum_Housing_DBT-EVAL.sql \
	sum/housing/Sum_Housing_DF-EVAL.m3
	
SUM_HOUSING_SOURCE_INCR = \
	sum/housing/Sum_Housing_DBT-INCR_ALL.sql \
	sum/housing/Sum_Housing_DF-INCR_ALL.m3 \
	sum/housing/Sum_Housing_DF-INCR_REEVAL_ALL.m3

SUM_HOUSING_GENERATE_HPP_FROM_SQL_EVAL = $(patsubst %.sql, $(BINDIR)/queries/%.hpp, $(filter %.sql, $(SUM_HOUSING_SOURCE_EVAL)))
SUM_HOUSING_GENERATE_HPP_FROM_SQL_INCR = $(patsubst %.sql, $(BINDIR)/queries/%.hpp, $(filter %.sql, $(SUM_HOUSING_SOURCE_INCR)))
SUM_HOUSING_GENERATE_HPP_FROM_M3_EVAL = $(patsubst %.m3, $(BINDIR)/queries/%.hpp, $(filter %.m3, $(SUM_HOUSING_SOURCE_EVAL)))
SUM_HOUSING_GENERATE_HPP_FROM_M3_INCR = $(patsubst %.m3, $(BINDIR)/queries/%.hpp, $(filter %.m3, $(SUM_HOUSING_SOURCE_INCR)))
SUM_HOUSING_GENERATE_HPP_FROM_HPP_EVAL = $(patsubst %.hpp, $(BINDIR)/queries/%.hpp, $(filter %.hpp, $(SUM_HOUSING_SOURCE_EVAL)))
SUM_HOUSING_GENERATE_HPP_FROM_HPP_INCR = $(patsubst %.hpp, $(BINDIR)/queries/%.hpp, $(filter %.hpp, $(SUM_HOUSING_SOURCE_INCR)))

SUM_HOUSING_GENERATE_HPP_EVAL = $(SUM_HOUSING_GENERATE_HPP_FROM_SQL_EVAL) $(SUM_HOUSING_GENERATE_HPP_FROM_M3_EVAL) $(SUM_HOUSING_GENERATE_HPP_FROM_HPP_EVAL)
SUM_HOUSING_GENERATE_HPP_INCR = $(SUM_HOUSING_GENERATE_HPP_FROM_SQL_INCR) $(SUM_HOUSING_GENERATE_HPP_FROM_M3_INCR) $(SUM_HOUSING_GENERATE_HPP_FROM_HPP_INCR)

SUM_HOUSING_EXE_EVAL = $(SUM_HOUSING_GENERATE_HPP_EVAL:$(BINDIR)/queries/%.hpp=$(BINDIR)/%) 
SUM_HOUSING_EXE_INCR = $(SUM_HOUSING_GENERATE_HPP_INCR:$(BINDIR)/queries/%.hpp=$(BINDIR)/%_$(BATCH_SIZE))


####################
# SUM RETAILER
####################
SUM_RETAILER_SOURCE_EVAL = \
	sum/retailer/Sum_Retailer_DBT-EVAL.sql \
	sum/retailer/Sum_Retailer_DF-EVAL.m3
	
SUM_RETAILER_SOURCE_INCR = \
	sum/retailer/Sum_Retailer_DBT-INCR_ALL.sql \
	sum/retailer/Sum_Retailer_DBT-INCR_INVENTORY.sql \
	sum/retailer/Sum_Retailer_DF-INCR_ALL.m3 \
	sum/retailer/Sum_Retailer_DF-INCR_INVENTORY.m3 \
	sum/retailer/Sum_Retailer_DF-INCR_REEVAL_ALL.m3 \
	sum/retailer/Sum_Retailer_DF-INCR_REEVAL_INVENTORY.m3

SUM_RETAILER_GENERATE_HPP_FROM_SQL_EVAL = $(patsubst %.sql, $(BINDIR)/queries/%.hpp, $(filter %.sql, $(SUM_RETAILER_SOURCE_EVAL)))
SUM_RETAILER_GENERATE_HPP_FROM_SQL_INCR = $(patsubst %.sql, $(BINDIR)/queries/%.hpp, $(filter %.sql, $(SUM_RETAILER_SOURCE_INCR)))
SUM_RETAILER_GENERATE_HPP_FROM_M3_EVAL = $(patsubst %.m3, $(BINDIR)/queries/%.hpp, $(filter %.m3, $(SUM_RETAILER_SOURCE_EVAL)))
SUM_RETAILER_GENERATE_HPP_FROM_M3_INCR = $(patsubst %.m3, $(BINDIR)/queries/%.hpp, $(filter %.m3, $(SUM_RETAILER_SOURCE_INCR)))
SUM_RETAILER_GENERATE_HPP_FROM_HPP_EVAL = $(patsubst %.hpp, $(BINDIR)/queries/%.hpp, $(filter %.hpp, $(SUM_RETAILER_SOURCE_EVAL)))
SUM_RETAILER_GENERATE_HPP_FROM_HPP_INCR = $(patsubst %.hpp, $(BINDIR)/queries/%.hpp, $(filter %.hpp, $(SUM_RETAILER_SOURCE_INCR)))

SUM_RETAILER_GENERATE_HPP_EVAL = $(SUM_RETAILER_GENERATE_HPP_FROM_SQL_EVAL) $(SUM_RETAILER_GENERATE_HPP_FROM_M3_EVAL) $(SUM_RETAILER_GENERATE_HPP_FROM_HPP_EVAL)
SUM_RETAILER_GENERATE_HPP_INCR = $(SUM_RETAILER_GENERATE_HPP_FROM_SQL_INCR) $(SUM_RETAILER_GENERATE_HPP_FROM_M3_INCR) $(SUM_RETAILER_GENERATE_HPP_FROM_HPP_INCR)

SUM_RETAILER_EXE_EVAL = $(SUM_RETAILER_GENERATE_HPP_EVAL:$(BINDIR)/queries/%.hpp=$(BINDIR)/%) 
SUM_RETAILER_EXE_INCR = $(SUM_RETAILER_GENERATE_HPP_INCR:$(BINDIR)/queries/%.hpp=$(BINDIR)/%_$(BATCH_SIZE))

####################
# SUM TPCH
####################
SUM_TPCH_SOURCE_EVAL = \
	sum/tpch/TPCH3_DBT-EVAL.sql \
	sum/tpch/TPCH3_DF-EVAL.m3 \
	sum/tpch/TPCH5_DBT-EVAL.sql \
	sum/tpch/TPCH5_DF-EVAL.m3 \
	sum/tpch/TPCH5_DF-EVAL_NOINDPROJ.m3 \
	sum/tpch/TPCH10_DBT-EVAL.sql \
	sum/tpch/TPCH10_DF-EVAL.m3 \
	sum/tpch/TPCH1_DBT-EVAL.sql \
	sum/tpch/TPCH1_DF-EVAL.hpp \
	sum/tpch/TPCH12_DBT-EVAL.sql \
	sum/tpch/TPCH12_DF-EVAL.hpp \
	sum/tpch/TPCH14_DBT-EVAL.sql \
	sum/tpch/TPCH14_DF-EVAL.hpp \
	sum/tpch/TPCH7_DBT-EVAL.sql \
	sum/tpch/TPCH7_DF-EVAL.m3 \
	sum/tpch/TPCH7_DF-EVAL_NOINDPROJ.m3 \
	sum/tpch/TPCH8_DBT-EVAL.sql \
	sum/tpch/TPCH8_DF-EVAL.hpp \
	sum/tpch/TPCH9_DBT-EVAL.sql \
	sum/tpch/TPCH9_DF-EVAL.hpp \
	sum/tpch/TPCH6_DBT-EVAL.sql \
	sum/tpch/TPCH19_DBT-EVAL.sql 
	
SUM_TPCH_SOURCE_INCR = \
	sum/tpch/TPCH3_DBT-INCR_ALL.sql \
	sum/tpch/TPCH3_DF-INCR_ALL.m3 \
	sum/tpch/TPCH3_DF-INCR_REEVAL_ALL.m3 \
	sum/tpch/TPCH5_DBT-INCR_ALL.sql \
	sum/tpch/TPCH5_DF-INCR_ALL.m3 \
	sum/tpch/TPCH5_DF-INCR_ALL_NOINDPROJ.m3 \
	sum/tpch/TPCH10_DBT-INCR_ALL.sql \
	sum/tpch/TPCH10_DF-INCR_ALL.m3 \
	sum/tpch/TPCH1_DBT-INCR_ALL.sql \
	sum/tpch/TPCH1_DF-INCR_ALL.hpp \
	sum/tpch/TPCH12_DBT-INCR_ALL.sql \
	sum/tpch/TPCH12_DF-INCR_ALL.hpp \
	sum/tpch/TPCH14_DBT-INCR_ALL.sql \
	sum/tpch/TPCH14_DF-INCR_ALL.hpp \
	sum/tpch/TPCH7_DBT-INCR_ALL.sql \
	sum/tpch/TPCH7_DF-INCR_ALL.m3 \
	sum/tpch/TPCH8_DBT-INCR_ALL.sql \
	sum/tpch/TPCH8_DF-INCR_ALL.hpp \
	sum/tpch/TPCH9_DBT-INCR_ALL.sql \
	sum/tpch/TPCH9_DF-INCR_ALL.hpp \
	sum/tpch/TPCH6_DBT-INCR_ALL.sql \
	sum/tpch/TPCH19_DBT-INCR_ALL.sql 

SUM_TPCH_GENERATE_HPP_FROM_SQL_EVAL = $(patsubst %.sql, $(BINDIR)/queries/%.hpp, $(filter %.sql, $(SUM_TPCH_SOURCE_EVAL)))
SUM_TPCH_GENERATE_HPP_FROM_SQL_INCR = $(patsubst %.sql, $(BINDIR)/queries/%.hpp, $(filter %.sql, $(SUM_TPCH_SOURCE_INCR)))
SUM_TPCH_GENERATE_HPP_FROM_M3_EVAL = $(patsubst %.m3, $(BINDIR)/queries/%.hpp, $(filter %.m3, $(SUM_TPCH_SOURCE_EVAL)))
SUM_TPCH_GENERATE_HPP_FROM_M3_INCR = $(patsubst %.m3, $(BINDIR)/queries/%.hpp, $(filter %.m3, $(SUM_TPCH_SOURCE_INCR)))
SUM_TPCH_GENERATE_HPP_FROM_HPP_EVAL = $(patsubst %.hpp, $(BINDIR)/queries/%.hpp, $(filter %.hpp, $(SUM_TPCH_SOURCE_EVAL)))
SUM_TPCH_GENERATE_HPP_FROM_HPP_INCR = $(patsubst %.hpp, $(BINDIR)/queries/%.hpp, $(filter %.hpp, $(SUM_TPCH_SOURCE_INCR)))

SUM_TPCH_GENERATE_HPP_EVAL = $(SUM_TPCH_GENERATE_HPP_FROM_SQL_EVAL) $(SUM_TPCH_GENERATE_HPP_FROM_M3_EVAL) $(SUM_TPCH_GENERATE_HPP_FROM_HPP_EVAL)
SUM_TPCH_GENERATE_HPP_INCR = $(SUM_TPCH_GENERATE_HPP_FROM_SQL_INCR) $(SUM_TPCH_GENERATE_HPP_FROM_M3_INCR) $(SUM_TPCH_GENERATE_HPP_FROM_HPP_INCR)

SUM_TPCH_EXE_EVAL = $(SUM_TPCH_GENERATE_HPP_EVAL:$(BINDIR)/queries/%.hpp=$(BINDIR)/%) 
SUM_TPCH_EXE_INCR = $(SUM_TPCH_GENERATE_HPP_INCR:$(BINDIR)/queries/%.hpp=$(BINDIR)/%_$(BATCH_SIZE))


####################
# FULL JOIN TPCH
####################
FULL_TPCH_SOURCE_EVAL = 
	
FULL_TPCH_SOURCE_INCR = \
	full_join/tpch/TPCH_FQ1.sql

FULL_TPCH_GENERATE_HPP_FROM_SQL_EVAL = $(patsubst %.sql, $(BINDIR)/queries/%.hpp, $(filter %.sql, $(FULL_TPCH_SOURCE_EVAL)))
FULL_TPCH_GENERATE_HPP_FROM_SQL_INCR = $(patsubst %.sql, $(BINDIR)/queries/%.hpp, $(filter %.sql, $(FULL_TPCH_SOURCE_INCR)))
FULL_TPCH_GENERATE_HPP_FROM_M3_EVAL = $(patsubst %.m3, $(BINDIR)/queries/%.hpp, $(filter %.m3, $(FULL_TPCH_SOURCE_EVAL)))
FULL_TPCH_GENERATE_HPP_FROM_M3_INCR = $(patsubst %.m3, $(BINDIR)/queries/%.hpp, $(filter %.m3, $(FULL_TPCH_SOURCE_INCR)))
FULL_TPCH_GENERATE_HPP_FROM_HPP_EVAL = $(patsubst %.hpp, $(BINDIR)/queries/%.hpp, $(filter %.hpp, $(FULL_TPCH_SOURCE_EVAL)))
FULL_TPCH_GENERATE_HPP_FROM_HPP_INCR = $(patsubst %.hpp, $(BINDIR)/queries/%.hpp, $(filter %.hpp, $(FULL_TPCH_SOURCE_INCR)))

FULL_TPCH_GENERATE_HPP_EVAL = $(FULL_TPCH_GENERATE_HPP_FROM_SQL_EVAL) $(FULL_TPCH_GENERATE_HPP_FROM_M3_EVAL) $(FULL_TPCH_GENERATE_HPP_FROM_HPP_EVAL)
FULL_TPCH_GENERATE_HPP_INCR = $(FULL_TPCH_GENERATE_HPP_FROM_SQL_INCR) $(FULL_TPCH_GENERATE_HPP_FROM_M3_INCR) $(FULL_TPCH_GENERATE_HPP_FROM_HPP_INCR)

FULL_TPCH_EXE_EVAL = $(FULL_TPCH_GENERATE_HPP_EVAL:$(BINDIR)/queries/%.hpp=$(BINDIR)/%) 
FULL_TPCH_EXE_INCR = $(FULL_TPCH_GENERATE_HPP_INCR:$(BINDIR)/queries/%.hpp=$(BINDIR)/%_$(BATCH_SIZE))

####################
# FULL JOIN HOUSING
####################
FULL_HOUSING_SOURCE_EVAL = \
	full_join/housing/Relational_Housing_DF-EVAL.hpp \
	full_join/housing/Factorized_Housing_DF-EVAL.hpp
	
FULL_HOUSING_SOURCE_INCR = \
	full_join/housing/Relational_Housing_DF-INCR_ALL.hpp \
	full_join/housing/Factorized_Housing_DF-INCR_ALL.hpp \
	full_join/housing/Factorized_Housing_DF-REEVAL_ALL.hpp

FULL_HOUSING_GENERATE_HPP_FROM_SQL_EVAL = $(patsubst %.sql, $(BINDIR)/queries/%.hpp, $(filter %.sql, $(FULL_HOUSING_SOURCE_EVAL)))
FULL_HOUSING_GENERATE_HPP_FROM_SQL_INCR = $(patsubst %.sql, $(BINDIR)/queries/%.hpp, $(filter %.sql, $(FULL_HOUSING_SOURCE_INCR)))
FULL_HOUSING_GENERATE_HPP_FROM_M3_EVAL = $(patsubst %.m3, $(BINDIR)/queries/%.hpp, $(filter %.m3, $(FULL_HOUSING_SOURCE_EVAL)))
FULL_HOUSING_GENERATE_HPP_FROM_M3_INCR = $(patsubst %.m3, $(BINDIR)/queries/%.hpp, $(filter %.m3, $(FULL_HOUSING_SOURCE_INCR)))
FULL_HOUSING_GENERATE_HPP_FROM_HPP_EVAL = $(patsubst %.hpp, $(BINDIR)/queries/%.hpp, $(filter %.hpp, $(FULL_HOUSING_SOURCE_EVAL)))
FULL_HOUSING_GENERATE_HPP_FROM_HPP_INCR = $(patsubst %.hpp, $(BINDIR)/queries/%.hpp, $(filter %.hpp, $(FULL_HOUSING_SOURCE_INCR)))

FULL_HOUSING_GENERATE_HPP_EVAL = $(FULL_HOUSING_GENERATE_HPP_FROM_SQL_EVAL) $(FULL_HOUSING_GENERATE_HPP_FROM_M3_EVAL) $(FULL_HOUSING_GENERATE_HPP_FROM_HPP_EVAL)
FULL_HOUSING_GENERATE_HPP_INCR = $(FULL_HOUSING_GENERATE_HPP_FROM_SQL_INCR) $(FULL_HOUSING_GENERATE_HPP_FROM_M3_INCR) $(FULL_HOUSING_GENERATE_HPP_FROM_HPP_INCR)

FULL_HOUSING_EXE_EVAL = $(FULL_HOUSING_GENERATE_HPP_EVAL:$(BINDIR)/queries/%.hpp=$(BINDIR)/%) 
FULL_HOUSING_EXE_INCR = $(FULL_HOUSING_GENERATE_HPP_INCR:$(BINDIR)/queries/%.hpp=$(BINDIR)/%_$(BATCH_SIZE))

####################
# FULL JOIN RETAILER
####################
FULL_RETAILER_SOURCE_EVAL = 
	
FULL_RETAILER_SOURCE_INCR = \
	full_join/retailer/Relational_Retailer_DF-INCR_INVENTORY_IMPROVED.hpp \
	full_join/retailer/Factorized_Retailer_DF-INCR_INVENTORY_IMPROVED.hpp \
	full_join/retailer/Relational_Retailer_DF-INCR_INVENTORY.hpp \
	full_join/retailer/Factorized_Retailer_DF-INCR_INVENTORY.hpp \
	full_join/retailer/Relational_Retailer_DF-INCR_ALL.hpp \
	full_join/retailer/Factorized_Retailer_DF-INCR_ALL.hpp

FULL_RETAILER_GENERATE_HPP_FROM_SQL_EVAL = $(patsubst %.sql, $(BINDIR)/queries/%.hpp, $(filter %.sql, $(FULL_RETAILER_SOURCE_EVAL)))
FULL_RETAILER_GENERATE_HPP_FROM_SQL_INCR = $(patsubst %.sql, $(BINDIR)/queries/%.hpp, $(filter %.sql, $(FULL_RETAILER_SOURCE_INCR)))
FULL_RETAILER_GENERATE_HPP_FROM_M3_EVAL = $(patsubst %.m3, $(BINDIR)/queries/%.hpp, $(filter %.m3, $(FULL_RETAILER_SOURCE_EVAL)))
FULL_RETAILER_GENERATE_HPP_FROM_M3_INCR = $(patsubst %.m3, $(BINDIR)/queries/%.hpp, $(filter %.m3, $(FULL_RETAILER_SOURCE_INCR)))
FULL_RETAILER_GENERATE_HPP_FROM_HPP_EVAL = $(patsubst %.hpp, $(BINDIR)/queries/%.hpp, $(filter %.hpp, $(FULL_RETAILER_SOURCE_EVAL)))
FULL_RETAILER_GENERATE_HPP_FROM_HPP_INCR = $(patsubst %.hpp, $(BINDIR)/queries/%.hpp, $(filter %.hpp, $(FULL_RETAILER_SOURCE_INCR)))

FULL_RETAILER_GENERATE_HPP_EVAL = $(FULL_RETAILER_GENERATE_HPP_FROM_SQL_EVAL) $(FULL_RETAILER_GENERATE_HPP_FROM_M3_EVAL) $(FULL_RETAILER_GENERATE_HPP_FROM_HPP_EVAL)
FULL_RETAILER_GENERATE_HPP_INCR = $(FULL_RETAILER_GENERATE_HPP_FROM_SQL_INCR) $(FULL_RETAILER_GENERATE_HPP_FROM_M3_INCR) $(FULL_RETAILER_GENERATE_HPP_FROM_HPP_INCR)

FULL_RETAILER_EXE_EVAL = $(FULL_RETAILER_GENERATE_HPP_EVAL:$(BINDIR)/queries/%.hpp=$(BINDIR)/%) 
FULL_RETAILER_EXE_INCR = $(FULL_RETAILER_GENERATE_HPP_INCR:$(BINDIR)/queries/%.hpp=$(BINDIR)/%_$(BATCH_SIZE))


####################
# MCM 
####################
MCM_SOURCE_EVAL = 
	
MCM_SOURCE_INCR = \
	mcm/MCM_DF-INCR_A2_row.hpp \
	mcm/MCM_DBT-INCR_A2_row.hpp \
	mcm/MCM_DBT-INCR_REEVAL_A2_row.hpp\
	mcm/MCM_DF-INCR_A2_rank1.hpp \
	mcm/MCM_DBT-INCR_A2_rank1.hpp \
	mcm/MCM_DBT-INCR_REEVAL_A2_rank1.hpp
	# mcm/MCM_DBT-INCR_A2_row_slower.hpp
	# mcm/MCM_DBT-INCR_A2.m3 \
	# mcm/MCM_DF-INCR_A2.m3 \
	# mcm/MCM_IVM-INCR_A2.m3 \
	# mcm/MCM_DBT-INCR_REEVAL_A2.m3

MCM_GENERATE_HPP_FROM_SQL_EVAL = $(patsubst %.sql, $(BINDIR)/queries/%.hpp, $(filter %.sql, $(MCM_SOURCE_EVAL)))
MCM_GENERATE_HPP_FROM_SQL_INCR = $(patsubst %.sql, $(BINDIR)/queries/%.hpp, $(filter %.sql, $(MCM_SOURCE_INCR)))
MCM_GENERATE_HPP_FROM_M3_EVAL = $(patsubst %.m3, $(BINDIR)/queries/%.hpp, $(filter %.m3, $(MCM_SOURCE_EVAL)))
MCM_GENERATE_HPP_FROM_M3_INCR = $(patsubst %.m3, $(BINDIR)/queries/%.hpp, $(filter %.m3, $(MCM_SOURCE_INCR)))
MCM_GENERATE_HPP_FROM_HPP_EVAL = $(patsubst %.hpp, $(BINDIR)/queries/%.hpp, $(filter %.hpp, $(MCM_SOURCE_EVAL)))
MCM_GENERATE_HPP_FROM_HPP_INCR = $(patsubst %.hpp, $(BINDIR)/queries/%.hpp, $(filter %.hpp, $(MCM_SOURCE_INCR)))

MCM_GENERATE_HPP_EVAL = $(MCM_GENERATE_HPP_FROM_SQL_EVAL) $(MCM_GENERATE_HPP_FROM_M3_EVAL) $(MCM_GENERATE_HPP_FROM_HPP_EVAL)
MCM_GENERATE_HPP_INCR = $(MCM_GENERATE_HPP_FROM_SQL_INCR) $(MCM_GENERATE_HPP_FROM_M3_INCR) $(MCM_GENERATE_HPP_FROM_HPP_INCR)

MCM_EXE_EVAL = $(MCM_GENERATE_HPP_EVAL:$(BINDIR)/queries/%.hpp=$(BINDIR)/%) 
MCM_EXE_INCR = $(MCM_GENERATE_HPP_INCR:$(BINDIR)/queries/%.hpp=$(BINDIR)/%)


####################
# CODE GENERATION
####################
GENERATE_HPP_FROM_SQL = \
	$(COFACTOR_HOUSING_GENERATE_HPP_FROM_SQL_EVAL) $(COFACTOR_HOUSING_GENERATE_HPP_FROM_SQL_INCR) \
	$(COFACTOR_RETAILER_GENERATE_HPP_FROM_SQL_EVAL) $(COFACTOR_RETAILER_GENERATE_HPP_FROM_SQL_INCR) \
	$(COFACTOR_TWITTER_GENERATE_HPP_FROM_SQL_EVAL) $(COFACTOR_TWITTER_GENERATE_HPP_FROM_SQL_INCR) \
	$(COFACTOR_TPCH_GENERATE_HPP_FROM_SQL_EVAL) $(COFACTOR_TPCH_GENERATE_HPP_FROM_SQL_INCR) \
	$(SUM_HOUSING_GENERATE_HPP_FROM_SQL_EVAL) $(SUM_HOUSING_GENERATE_HPP_FROM_SQL_INCR) \
	$(SUM_RETAILER_GENERATE_HPP_FROM_SQL_EVAL) $(SUM_RETAILER_GENERATE_HPP_FROM_SQL_INCR) \
	$(SUM_TPCH_GENERATE_HPP_FROM_SQL_EVAL) $(SUM_TPCH_GENERATE_HPP_FROM_SQL_INCR) \
	$(FULL_TPCH_GENERATE_HPP_FROM_SQL_EVAL) $(FULL_TPCH_GENERATE_HPP_FROM_SQL_INCR) \
	$(FULL_HOUSING_GENERATE_HPP_FROM_SQL_EVAL) $(FULL_HOUSING_GENERATE_HPP_FROM_SQL_INCR) \
	$(FULL_RETAILER_GENERATE_HPP_FROM_SQL_EVAL) $(FULL_RETAILER_GENERATE_HPP_FROM_SQL_INCR) \
	$(MCM_GENERATE_HPP_FROM_SQL_EVAL) $(MCM_GENERATE_HPP_FROM_SQL_INCR)

GENERATE_HPP_FROM_M3 = \
	$(COFACTOR_HOUSING_GENERATE_HPP_FROM_M3_EVAL) $(COFACTOR_HOUSING_GENERATE_HPP_FROM_M3_INCR) \
	$(COFACTOR_RETAILER_GENERATE_HPP_FROM_M3_EVAL) $(COFACTOR_RETAILER_GENERATE_HPP_FROM_M3_INCR)	\
	$(COFACTOR_TWITTER_GENERATE_HPP_FROM_M3_EVAL) $(COFACTOR_TWITTER_GENERATE_HPP_FROM_M3_INCR) \
	$(COFACTOR_TPCH_GENERATE_HPP_FROM_M3_EVAL) $(COFACTOR_TPCH_GENERATE_HPP_FROM_M3_INCR)	\
	$(SUM_HOUSING_GENERATE_HPP_FROM_M3_EVAL) $(SUM_HOUSING_GENERATE_HPP_FROM_M3_INCR) \
	$(SUM_RETAILER_GENERATE_HPP_FROM_M3_EVAL) $(SUM_RETAILER_GENERATE_HPP_FROM_M3_INCR) \
	$(SUM_TPCH_GENERATE_HPP_FROM_M3_EVAL) $(SUM_TPCH_GENERATE_HPP_FROM_M3_INCR) \
	$(FULL_TPCH_GENERATE_HPP_FROM_M3_EVAL) $(FULL_TPCH_GENERATE_HPP_FROM_M3_INCR) \
	$(FULL_HOUSING_GENERATE_HPP_FROM_M3_EVAL) $(FULL_HOUSING_GENERATE_HPP_FROM_M3_INCR) \
	$(FULL_RETAILER_GENERATE_HPP_FROM_M3_EVAL) $(FULL_RETAILER_GENERATE_HPP_FROM_M3_INCR) \
	$(MCM_GENERATE_HPP_FROM_M3_EVAL) $(MCM_GENERATE_HPP_FROM_M3_INCR)

GENERATE_HPP_FROM_HPP = \
	$(COFACTOR_HOUSING_GENERATE_HPP_FROM_HPP_EVAL) $(COFACTOR_HOUSING_GENERATE_HPP_FROM_HPP_INCR) \
	$(COFACTOR_RETAILER_GENERATE_HPP_FROM_HPP_EVAL) $(COFACTOR_RETAILER_GENERATE_HPP_FROM_HPP_INCR) \
	$(COFACTOR_TWITTER_GENERATE_HPP_FROM_HPP_EVAL) $(COFACTOR_TWITTER_GENERATE_HPP_FROM_HPP_INCR) \
	$(COFACTOR_TPCH_GENERATE_HPP_FROM_HPP_EVAL) $(COFACTOR_TPCH_GENERATE_HPP_FROM_HPP_INCR) \
	$(SUM_HOUSING_GENERATE_HPP_FROM_HPP_EVAL) $(SUM_HOUSING_GENERATE_HPP_FROM_HPP_INCR) \
	$(SUM_RETAILER_GENERATE_HPP_FROM_HPP_EVAL) $(SUM_RETAILER_GENERATE_HPP_FROM_HPP_INCR) \
	$(SUM_TPCH_GENERATE_HPP_FROM_HPP_EVAL) $(SUM_TPCH_GENERATE_HPP_FROM_HPP_INCR) \
	$(FULL_TPCH_GENERATE_HPP_FROM_HPP_EVAL) $(FULL_TPCH_GENERATE_HPP_FROM_HPP_INCR) \
	$(FULL_HOUSING_GENERATE_HPP_FROM_HPP_EVAL) $(FULL_HOUSING_GENERATE_HPP_FROM_HPP_INCR) \
	$(FULL_RETAILER_GENERATE_HPP_FROM_HPP_EVAL) $(FULL_RETAILER_GENERATE_HPP_FROM_HPP_INCR) \
	$(MCM_GENERATE_HPP_FROM_HPP_EVAL) $(MCM_GENERATE_HPP_FROM_HPP_INCR)
	

# $(info $(COFACTOR_RETAILER_EXE_INCR))

.PHONY: all codegen cofactor_housing cofactor_retailer cofactor_twitter cofactor_tpch sum_housing sum_retailer full_tpch mcm

all: cofactor_housing cofactor_retailer cofactor_twitter cofactor_tpch

codegen: $(GENERATE_HPP_FROM_SQL) $(GENERATE_HPP_FROM_M3) $(GENERATE_HPP_FROM_HPP)

##########

cofactor_housing: $(COFACTOR_HOUSING_EXE_EVAL) $(COFACTOR_HOUSING_EXE_INCR)

$(COFACTOR_HOUSING_EXE_EVAL): $(BINDIR)/% : $(BINDIR)/queries/%.hpp src/templates/housing_template.hpp
	@test -d $(dir $@) || mkdir -p $(dir $@)
	$(CC) $(CFLAGS) src/main.cpp -I src/lib -DNDEBUG -O3 -DNUMBER_OF_RUNS=$(NUMBER_OF_RUNS) $(FLAGS) -include $< -include src/templates/housing_template.hpp -o $@ 

$(COFACTOR_HOUSING_EXE_INCR): $(BINDIR)/%_$(BATCH_SIZE) : $(BINDIR)/queries/%.hpp src/templates/housing_template.hpp
	@test -d $(dir $@) || mkdir -p $(dir $@)
	$(CC) $(CFLAGS) src/main.cpp -I src/lib -DNDEBUG -O3 -DNUMBER_OF_RUNS=$(NUMBER_OF_RUNS) -DBATCH_SIZE=$(BATCH_SIZE) $(FLAGS) -include $< -include src/templates/housing_template.hpp -o $@ 

##########

cofactor_retailer: $(COFACTOR_RETAILER_EXE_EVAL) $(COFACTOR_RETAILER_EXE_INCR)

$(COFACTOR_RETAILER_EXE_EVAL): $(BINDIR)/% : $(BINDIR)/queries/%.hpp src/templates/retailer_template.hpp
	@test -d $(dir $@) || mkdir -p $(dir $@)
	$(CC) $(CFLAGS) src/main.cpp -I src/lib -DNDEBUG -O3 -DNUMBER_OF_RUNS=$(NUMBER_OF_RUNS) $(FLAGS) -include $< -include src/templates/retailer_template.hpp -o $@ 

$(COFACTOR_RETAILER_EXE_INCR): $(BINDIR)/%_$(BATCH_SIZE) : $(BINDIR)/queries/%.hpp src/templates/retailer_template.hpp
	@test -d $(dir $@) || mkdir -p $(dir $@)
	$(CC) $(CFLAGS) src/main.cpp -I src/lib -DNDEBUG -O3 -DNUMBER_OF_RUNS=$(NUMBER_OF_RUNS) -DBATCH_SIZE=$(BATCH_SIZE) $(FLAGS) -include $< -include src/templates/retailer_template.hpp -o $@ 

##########

cofactor_twitter: $(COFACTOR_TWITTER_EXE_EVAL) $(COFACTOR_TWITTER_EXE_INCR)

$(COFACTOR_TWITTER_EXE_EVAL): $(BINDIR)/% : $(BINDIR)/queries/%.hpp src/templates/twitter_template.hpp
	@test -d $(dir $@) || mkdir -p $(dir $@)
	$(CC) $(CFLAGS) src/main.cpp -I src/lib -DNDEBUG -O3 -DNUMBER_OF_RUNS=$(NUMBER_OF_RUNS) $(FLAGS) -include $< -include src/templates/twitter_template.hpp -o $@ 

$(COFACTOR_TWITTER_EXE_INCR): $(BINDIR)/%_$(BATCH_SIZE) : $(BINDIR)/queries/%.hpp src/templates/twitter_template.hpp
	@test -d $(dir $@) || mkdir -p $(dir $@)
	$(CC) $(CFLAGS) src/main.cpp -I src/lib -DNDEBUG -O3 -DNUMBER_OF_RUNS=$(NUMBER_OF_RUNS) -DBATCH_SIZE=$(BATCH_SIZE) $(FLAGS) -include $< -include src/templates/twitter_template.hpp -o $@ 

##########

cofactor_tpch: $(COFACTOR_TPCH_EXE_EVAL) $(COFACTOR_TPCH_EXE_INCR)

$(COFACTOR_TPCH_EXE_EVAL): $(BINDIR)/% : $(BINDIR)/queries/%.hpp src/templates/tpch_template.hpp
	@test -d $(dir $@) || mkdir -p $(dir $@)
	$(CC) $(CFLAGS) src/main.cpp -I src/lib -DNDEBUG -O3 -DNUMBER_OF_RUNS=$(NUMBER_OF_RUNS) $(FLAGS) -include $< -include src/templates/tpch_template.hpp -o $@ 

$(COFACTOR_TPCH_EXE_INCR): $(BINDIR)/%_$(BATCH_SIZE) : $(BINDIR)/queries/%.hpp src/templates/tpch_template.hpp
	@test -d $(dir $@) || mkdir -p $(dir $@)
	$(CC) $(CFLAGS) src/main.cpp -I src/lib -DNDEBUG -O3 -DNUMBER_OF_RUNS=$(NUMBER_OF_RUNS) -DBATCH_SIZE=$(BATCH_SIZE) $(FLAGS) -include $< -include src/templates/tpch_template.hpp -o $@ 

##########

sum_housing: $(SUM_HOUSING_EXE_EVAL) $(SUM_HOUSING_EXE_INCR)

$(SUM_HOUSING_EXE_EVAL): $(BINDIR)/% : $(BINDIR)/queries/%.hpp src/templates/housing_template.hpp
	@test -d $(dir $@) || mkdir -p $(dir $@)
	$(CC) $(CFLAGS) src/main.cpp -I src/lib -DNDEBUG -O3 -DNUMBER_OF_RUNS=$(NUMBER_OF_RUNS) $(FLAGS) -include $< -include src/templates/housing_template.hpp -o $@ 

$(SUM_HOUSING_EXE_INCR): $(BINDIR)/%_$(BATCH_SIZE) : $(BINDIR)/queries/%.hpp src/templates/housing_template.hpp
	@test -d $(dir $@) || mkdir -p $(dir $@)
	$(CC) $(CFLAGS) src/main.cpp -I src/lib -DNDEBUG -O3 -DNUMBER_OF_RUNS=$(NUMBER_OF_RUNS) -DBATCH_SIZE=$(BATCH_SIZE) $(FLAGS) -include $< -include src/templates/housing_template.hpp -o $@ 

##########

sum_retailer: $(SUM_RETAILER_EXE_EVAL) $(SUM_RETAILER_EXE_INCR)

$(SUM_RETAILER_EXE_EVAL): $(BINDIR)/% : $(BINDIR)/queries/%.hpp src/templates/retailer_template.hpp
	@test -d $(dir $@) || mkdir -p $(dir $@)
	$(CC) $(CFLAGS) src/main.cpp -I src/lib -DNDEBUG -O3 -DNUMBER_OF_RUNS=$(NUMBER_OF_RUNS) $(FLAGS) -include $< -include src/templates/retailer_template.hpp -o $@ 

$(SUM_RETAILER_EXE_INCR): $(BINDIR)/%_$(BATCH_SIZE) : $(BINDIR)/queries/%.hpp src/templates/retailer_template.hpp
	@test -d $(dir $@) || mkdir -p $(dir $@)
	$(CC) $(CFLAGS) src/main.cpp -I src/lib -DNDEBUG -O3 -DNUMBER_OF_RUNS=$(NUMBER_OF_RUNS) -DBATCH_SIZE=$(BATCH_SIZE) $(FLAGS) -include $< -include src/templates/retailer_template.hpp -o $@ 

##########

sum_tpch: $(SUM_TPCH_EXE_EVAL) $(SUM_TPCH_EXE_INCR)

$(SUM_TPCH_EXE_EVAL): $(BINDIR)/% : $(BINDIR)/queries/%.hpp src/templates/tpch_template.hpp
	@test -d $(dir $@) || mkdir -p $(dir $@)
	$(CC) $(CFLAGS) src/main.cpp -I src/lib -DNDEBUG -O3 -DNUMBER_OF_RUNS=$(NUMBER_OF_RUNS) $(FLAGS) -include $< -include src/templates/tpch_template.hpp -o $@ 

$(SUM_TPCH_EXE_INCR): $(BINDIR)/%_$(BATCH_SIZE) : $(BINDIR)/queries/%.hpp src/templates/tpch_template.hpp
	@test -d $(dir $@) || mkdir -p $(dir $@)
	$(CC) $(CFLAGS) src/main.cpp -I src/lib -DNDEBUG -O3 -DNUMBER_OF_RUNS=$(NUMBER_OF_RUNS) -DBATCH_SIZE=$(BATCH_SIZE) $(FLAGS) -include $< -include src/templates/tpch_template.hpp -o $@ 

##########

full_tpch: $(FULL_TPCH_EXE_EVAL) $(FULL_TPCH_EXE_INCR)

$(FULL_TPCH_EXE_EVAL): $(BINDIR)/% : $(BINDIR)/queries/%.hpp src/templates/tpch_template.hpp
	@test -d $(dir $@) || mkdir -p $(dir $@)
	$(CC) $(CFLAGS) src/main.cpp -I src/lib -DNDEBUG -O3 -DNUMBER_OF_RUNS=$(NUMBER_OF_RUNS) $(FLAGS) -include $< -include src/templates/tpch_template.hpp -o $@ 

$(FULL_TPCH_EXE_INCR): $(BINDIR)/%_$(BATCH_SIZE) : $(BINDIR)/queries/%.hpp src/templates/tpch_template.hpp
	@test -d $(dir $@) || mkdir -p $(dir $@)
	$(CC) $(CFLAGS) src/main.cpp -I src/lib -DNDEBUG -O3 -DNUMBER_OF_RUNS=$(NUMBER_OF_RUNS) -DBATCH_SIZE=$(BATCH_SIZE) $(FLAGS) -include $< -include src/templates/tpch_template.hpp -o $@ 


##########

full_housing: $(FULL_HOUSING_EXE_EVAL) $(FULL_HOUSING_EXE_INCR)

$(FULL_HOUSING_EXE_EVAL): $(BINDIR)/% : $(BINDIR)/queries/%.hpp src/templates/housing_template.hpp
	@test -d $(dir $@) || mkdir -p $(dir $@)
	$(CC) $(CFLAGS) src/main.cpp -I src/lib -DNDEBUG -O3 -DNUMBER_OF_RUNS=$(NUMBER_OF_RUNS) $(FLAGS) -include $< -include src/templates/housing_template.hpp -o $@ 

$(FULL_HOUSING_EXE_INCR): $(BINDIR)/%_$(BATCH_SIZE) : $(BINDIR)/queries/%.hpp src/templates/housing_template.hpp
	@test -d $(dir $@) || mkdir -p $(dir $@)
	$(CC) $(CFLAGS) src/main.cpp -I src/lib -DNDEBUG -O3 -DNUMBER_OF_RUNS=$(NUMBER_OF_RUNS) -DBATCH_SIZE=$(BATCH_SIZE) $(FLAGS) -include $< -include src/templates/housing_template.hpp -o $@ 

##########

full_retailer: $(FULL_RETAILER_EXE_EVAL) $(FULL_RETAILER_EXE_INCR)

$(FULL_RETAILER_EXE_EVAL): $(BINDIR)/% : $(BINDIR)/queries/%.hpp src/templates/retailer_template.hpp
	@test -d $(dir $@) || mkdir -p $(dir $@)
	$(CC) $(CFLAGS) src/main.cpp -I src/lib -DNDEBUG -O3 -DNUMBER_OF_RUNS=$(NUMBER_OF_RUNS) $(FLAGS) -include $< -include src/templates/retailer_template.hpp -o $@ 

$(FULL_RETAILER_EXE_INCR): $(BINDIR)/%_$(BATCH_SIZE) : $(BINDIR)/queries/%.hpp src/templates/retailer_template.hpp
	@test -d $(dir $@) || mkdir -p $(dir $@)
	$(CC) $(CFLAGS) src/main.cpp -I src/lib -DNDEBUG -O3 -DNUMBER_OF_RUNS=$(NUMBER_OF_RUNS) -DBATCH_SIZE=$(BATCH_SIZE) $(FLAGS) -include $< -include src/templates/retailer_template.hpp -o $@ 

##########

mcm: $(MCM_EXE_EVAL) $(MCM_EXE_INCR)

$(MCM_EXE_EVAL): $(BINDIR)/% : $(BINDIR)/queries/%.hpp src/templates/mcm_template.hpp
	@test -d $(dir $@) || mkdir -p $(dir $@)
	$(CC) $(CFLAGS) src/main.cpp -I src/lib -DNDEBUG -O3 -DNUMBER_OF_RUNS=$(NUMBER_OF_RUNS) $(FLAGS) -include $< -include src/templates/mcm_template.hpp -o $@ 

$(MCM_EXE_INCR): $(BINDIR)/% : $(BINDIR)/queries/%.hpp src/templates/mcm_template.hpp
	@test -d $(dir $@) || mkdir -p $(dir $@)
	$(CC) $(CFLAGS) src/main.cpp -I src/lib -DNDEBUG -O3 -DNUMBER_OF_RUNS=$(NUMBER_OF_RUNS) -DBATCH_SIZE=$(BATCH_SIZE) $(FLAGS) -include $< -include src/templates/mcm_template.hpp -o $@ 

##########


$(GENERATE_HPP_FROM_SQL): $(BINDIR)/queries/%.hpp : $(SRC_QUERIES_DIR)/%.sql
	$(DBTOASTER) $(DBTOASTER_FLAGS) --batch -l vcpp -O3 -o $@ $<

$(GENERATE_HPP_FROM_M3): $(BINDIR)/queries/%.hpp : $(SRC_QUERIES_DIR)/%.m3
	$(DBTOASTER) $(DBTOASTER_FLAGS) --batch -l vcpp -O3 -o $@ $<

$(GENERATE_HPP_FROM_HPP): $(BINDIR)/queries/%.hpp : $(SRC_QUERIES_DIR)/%.hpp
	@test -d $(dir $@) || mkdir -p $(dir $@)
	cp $< $@

clean: 
	rm -fr $(BINDIR)
