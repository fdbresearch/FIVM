#!/bin/bash

export HEAP_PROFILE_INUSE_INTERVAL=0 
export HEAP_PROFILE_ALLOCATION_INTERVAL=0

export CPP_FLAGS="-L/opt/local/lib"

TIMEOUT=3600
TIMEOUT_BIN="timeout"

unamestr=`uname`
if [[ "$unamestr" == 'Linux' ]]; then
   PPROF_BIN="google-pprof"
else
   PPROF_BIN="pprof"
fi

function process_heap_profiles() {
  for file in `ls *.heap 2> /dev/null`
  do
    echo "${file}: `${PPROF_BIN} --text --inuse_space $1 ${file} 2> /dev/null | grep -i Total`"
  done
  
  mkdir -p memory_logs/$1
  rm -f memory_logs/$1/*.heap
  mv *.heap memory_logs/$1/ 2> /dev/null
}

function run() {
  BINARY=$1
  NUMBER_OF_RUNS=$2
  MAKE_FLAGS=$3
  RUNTIME_FLAGS=$4
  BATCH_SIZE=$5

  rm -f ${BINARY}
  echo "make ${BINARY} NUMBER_OF_RUNS=${NUMBER_OF_RUNS} FLAGS=\"${MAKE_FLAGS} ${CPP_FLAGS}\" BATCH_SIZE=${BATCH_SIZE} "
  make ${BINARY} NUMBER_OF_RUNS=${NUMBER_OF_RUNS} FLAGS="${MAKE_FLAGS} ${CPP_FLAGS}" BATCH_SIZE=${BATCH_SIZE} 

  echo "${TIMEOUT_BIN} ${TIMEOUT} ${BINARY} ${RUNTIME_FLAGS}"
  ${TIMEOUT_BIN} ${TIMEOUT} ${BINARY} ${RUNTIME_FLAGS}

  process_heap_profiles ${BINARY}
}

####################
# COFACTOR HOUSING
####################
function run_cofactor_housing() {
  # for scale in {1..20}
  for scale in 20
  do
    if [[ "$unamestr" == 'Linux' ]]; then
       sed -i "s/datasets\/housing-.*-normalised/datasets\/housing-${scale}-normalised/g" src/templates/housing_template.hpp 
    else
       sed -i '' "s/datasets\/housing-.*-normalised/datasets\/housing-${scale}-normalised/g" src/templates/housing_template.hpp 
    fi

    echo "-----------------------------------------"
    echo "SCALE: ${scale}"

    # run "bin/cofactor/housing/Cofactor_Housing_DF-EVAL" 4 "" "--no-output"
    # run "bin/cofactor/housing/Cofactor_Housing_DBT-EVAL" 1 "" "--no-output"
    # run "bin/cofactor/housing/Cofactor_Housing_PIVOT-EVAL" 4 "" "--no-output"

    # for bs in 100 1000 10000 100000
    for bs in 1000
    do 
      run "bin/cofactor/housing/Cofactor_Housing_DF-INCR_ALL_${bs}" 4 "" "--no-output" ${bs}
      # run "bin/cofactor/housing/Cofactor_Housing_DBT-INCR_ALL_${bs}" 4 "" "--no-output" ${bs}
      # run "bin/cofactor/housing/Cofactor_Housing_PIVOT-INCR_ALL_${bs}" 4 "" "--no-output" ${bs}

      # run "bin/cofactor/housing/Cofactor_Housing_DF-INCR_ALL_${bs}" 1 "-DLOG_OUTPUT=$((500 * ${scale}))" "--no-output" ${bs}
      # run "bin/cofactor/housing/Cofactor_Housing_DBT-INCR_ALL_${bs}" 1 "-DLOG_OUTPUT=$((500 * ${scale}))" "--no-output" ${bs}
      # run "bin/cofactor/housing/Cofactor_Housing_PIVOT-INCR_ALL_${bs}" 1 "-DLOG_OUTPUT=$((500 * ${scale}))" "--no-output" ${bs}

      # run "bin/cofactor/housing/Cofactor_Housing_DF-INCR_ALL_${bs}" 1 "-ltcmalloc -DLOG_MEMORY_INFO -DLOG_OUTPUT=$((500 * ${scale}))" "--no-output" ${bs}
      # run "bin/cofactor/housing/Cofactor_Housing_DBT-INCR_ALL_${bs}" 1 "-ltcmalloc -DLOG_MEMORY_INFO -DLOG_OUTPUT=$((500 * ${scale}))" "--no-output" ${bs}
      # run "bin/cofactor/housing/Cofactor_Housing_PIVOT-INCR_ALL_${bs}" 1 "-ltcmalloc -DLOG_MEMORY_INFO -DLOG_OUTPUT=$((500 * ${scale}))" "--no-output" ${bs}
    done
  done
}

####################
# COFACTOR RETAILER
####################
function run_cofactor_retailer() {
  run "bin/cofactor/retailer/Cofactor_Retailer_DF-EVAL" 4 "" "--no-output"
  run "bin/cofactor/retailer/Cofactor_Retailer_DBT-EVAL" 1 "" "--no-output"
  run "bin/cofactor/retailer/Cofactor_Retailer_PIVOT-EVAL" 4 "" "--no-output"

  # for bs in 100 1000 10000 100000
  for bs in 1000
  do 
    run "bin/cofactor/retailer/Cofactor_Retailer_DF-INCR_ALL_${bs}" 4 "" "--no-output" ${bs}
    run "bin/cofactor/retailer/Cofactor_Retailer_DBT-INCR_ALL_${bs}" 1 "" "--no-output" ${bs}
    run "bin/cofactor/retailer/Cofactor_Retailer_PIVOT-INCR_ALL_${bs}" 4 "" "--no-output" ${bs}
    run "bin/cofactor/retailer/Cofactor_Retailer_DF-INCR_INVENTORY_${bs}" 4 "" "--no-output" ${bs}
    run "bin/cofactor/retailer/Cofactor_Retailer_DBT-INCR_INVENTORY_${bs}" 1 "" "--no-output" ${bs}
    run "bin/cofactor/retailer/Cofactor_Retailer_PIVOT-INCR_INVENTORY_${bs}" 4 "" "--no-output" ${bs}

    run "bin/cofactor/retailer/Cofactor_Retailer_DF-INCR_ALL_${bs}" 1 "-DLOG_OUTPUT=500000" "--no-output" ${bs}
    run "bin/cofactor/retailer/Cofactor_Retailer_DBT-INCR_ALL_${bs}" 1 "-DLOG_OUTPUT=500000" "--no-output" ${bs}
    run "bin/cofactor/retailer/Cofactor_Retailer_PIVOT-INCR_ALL_${bs}" 1 "-DLOG_OUTPUT=500000" "--no-output" ${bs}
    run "bin/cofactor/retailer/Cofactor_Retailer_DF-INCR_INVENTORY_${bs}" 1 "-DLOG_OUTPUT=500000" "--no-output" ${bs}
    run "bin/cofactor/retailer/Cofactor_Retailer_DBT-INCR_INVENTORY_${bs}" 1 "-DLOG_OUTPUT=500000" "--no-output" ${bs}
    run "bin/cofactor/retailer/Cofactor_Retailer_PIVOT-INCR_INVENTORY_${bs}" 1 "-DLOG_OUTPUT=500000" "--no-output" ${bs}

    run "bin/cofactor/retailer/Cofactor_Retailer_DF-INCR_ALL_${bs}" 1 "-ltcmalloc -DLOG_MEMORY_INFO -DLOG_OUTPUT=500000" "--no-output" ${bs}
    run "bin/cofactor/retailer/Cofactor_Retailer_DBT-INCR_ALL_${bs}" 1 "-ltcmalloc -DLOG_MEMORY_INFO -DLOG_OUTPUT=500000" "--no-output" ${bs}
    run "bin/cofactor/retailer/Cofactor_Retailer_PIVOT-INCR_ALL_${bs}" 1 "-ltcmalloc -DLOG_MEMORY_INFO -DLOG_OUTPUT=500000" "--no-output" ${bs}
    run "bin/cofactor/retailer/Cofactor_Retailer_DF-INCR_INVENTORY_${bs}" 1 "-ltcmalloc -DLOG_MEMORY_INFO -DLOG_OUTPUT=500000" "--no-output" ${bs}
    run "bin/cofactor/retailer/Cofactor_Retailer_DBT-INCR_INVENTORY_${bs}" 1 "-ltcmalloc -DLOG_MEMORY_INFO -DLOG_OUTPUT=500000" "--no-output" ${bs}
    run "bin/cofactor/retailer/Cofactor_Retailer_PIVOT-INCR_INVENTORY_${bs}" 1 "-ltcmalloc -DLOG_MEMORY_INFO -DLOG_OUTPUT=500000" "--no-output" ${bs}
  done
}

####################
# COFACTOR TWITTER
####################
function run_cofactor_twitter() {
  run "bin/cofactor/twitter/Cofactor_Triangle_DF-EVAL" 4 "" "--no-output"
  run "bin/cofactor/twitter/Cofactor_Triangle_DBT-EVAL" 4 "" "--no-output"
  run "bin/cofactor/twitter/Cofactor_Triangle_PIVOT-EVAL" 4 "" "--no-output"

  # for bs in 100 1000 10000 100000
  for bs in 1000
  do 
    run "bin/cofactor/twitter/Cofactor_Triangle_DF-INCR_R_S_T_${bs}" 4 "" "--no-output" ${bs}
    run "bin/cofactor/twitter/Cofactor_Triangle_DBT-INCR_R_S_T_${bs}" 4 "" "--no-output" ${bs}
    run "bin/cofactor/twitter/Cofactor_Triangle_PIVOT-INCR_R_S_T_${bs}" 4 "" "--no-output" ${bs}
    run "bin/cofactor/twitter/Cofactor_Triangle_DF-INCR_R_${bs}" 4 "" "--no-output" ${bs}
    run "bin/cofactor/twitter/Cofactor_Triangle_DBT-INCR_R_${bs}" 4 "" "--no-output" ${bs}
    run "bin/cofactor/twitter/Cofactor_Triangle_PIVOT-INCR_R_${bs}" 4 "" "--no-output" ${bs}

    run "bin/cofactor/twitter/Cofactor_Triangle_DF-INCR_R_S_T_${bs}" 1 "-DLOG_OUTPUT=30000" "--no-output" ${bs}
    run "bin/cofactor/twitter/Cofactor_Triangle_DBT-INCR_R_S_T_${bs}" 1 "-DLOG_OUTPUT=30000" "--no-output" ${bs}
    run "bin/cofactor/twitter/Cofactor_Triangle_PIVOT-INCR_R_S_T_${bs}" 1 "-DLOG_OUTPUT=30000" "--no-output" ${bs}
    run "bin/cofactor/twitter/Cofactor_Triangle_DF-INCR_R_${bs}" 1 "-DLOG_OUTPUT=30000" "--no-output" ${bs}
    run "bin/cofactor/twitter/Cofactor_Triangle_DBT-INCR_R_${bs}" 1 "-DLOG_OUTPUT=30000" "--no-output" ${bs}
    run "bin/cofactor/twitter/Cofactor_Triangle_PIVOT-INCR_R_${bs}" 1 "-DLOG_OUTPUT=30000" "--no-output" ${bs}

    run "bin/cofactor/twitter/Cofactor_Triangle_DF-INCR_R_S_T_${bs}" 1 "-ltcmalloc -DLOG_MEMORY_INFO -DLOG_OUTPUT=30000" "--no-output" ${bs}
    run "bin/cofactor/twitter/Cofactor_Triangle_DBT-INCR_R_S_T_${bs}" 1 "-ltcmalloc -DLOG_MEMORY_INFO -DLOG_OUTPUT=30000" "--no-output" ${bs}
    run "bin/cofactor/twitter/Cofactor_Triangle_PIVOT-INCR_R_S_T_${bs}" 1 "-ltcmalloc -DLOG_MEMORY_INFO -DLOG_OUTPUT=30000" "--no-output" ${bs}
    run "bin/cofactor/twitter/Cofactor_Triangle_DF-INCR_R_${bs}" 1 "-ltcmalloc -DLOG_MEMORY_INFO -DLOG_OUTPUT=30000" "--no-output" ${bs}
    run "bin/cofactor/twitter/Cofactor_Triangle_DBT-INCR_R_${bs}" 1 "-ltcmalloc -DLOG_MEMORY_INFO -DLOG_OUTPUT=30000" "--no-output" ${bs}
    run "bin/cofactor/twitter/Cofactor_Triangle_PIVOT-INCR_R_${bs}" 1 "-ltcmalloc -DLOG_MEMORY_INFO -DLOG_OUTPUT=30000" "--no-output" ${bs}
  done
}

####################
# COFACTOR TPCH
####################
function run_cofactor_tpch() {

  # for bs in 100 1000 10000 100000
  for bs in 1000
  do 
    run "bin/cofactor/tpch/Cofactor_TPCH9_DF-INCR_ALL_SIMPLIFIED_${bs}" 4 "" "--no-output" ${bs}   
    run "bin/cofactor/tpch/Cofactor_TPCH9_DF-INCR_ALL_SIMPLIFIED_${bs}" 1 "-DLOG_OUTPUT=100000" "--no-output" ${bs}
    run "bin/cofactor/tpch/Cofactor_TPCH9_DF-INCR_ALL_SIMPLIFIED_${bs}" 1 "-ltcmalloc -DLOG_MEMORY_INFO -DLOG_OUTPUT=100000" "--no-output" ${bs}

    run "bin/cofactor/tpch/Cofactor_TPCH9_DBT-INCR_ALL_SIMPLIFIED_${bs}" 1 "" "--no-output" ${bs}
    run "bin/cofactor/tpch/Cofactor_TPCH9_DBT-INCR_ALL_SIMPLIFIED_${bs}" 1 "-DLOG_OUTPUT=100000" "--no-output" ${bs}
    run "bin/cofactor/tpch/Cofactor_TPCH9_DBT-INCR_ALL_SIMPLIFIED_${bs}" 1 "-ltcmalloc -DLOG_MEMORY_INFO -DLOG_OUTPUT=100000" "--no-output" ${bs}
  done
}

####################
# SUM HOUSING
####################
function run_sum_housing() {
  # for scale in {1..20}
  for scale in 20
  do
    if [[ "$unamestr" == 'Linux' ]]; then
       sed -i "s/datasets\/housing-.*-normalised/datasets\/housing-${scale}-normalised/g" src/templates/housing_template.hpp 
    else
       sed -i '' "s/datasets\/housing-.*-normalised/datasets\/housing-${scale}-normalised/g" src/templates/housing_template.hpp 
    fi

    echo "-----------------------------------------"
    echo "SCALE: ${scale}"

    # run "bin/sum/housing/Sum_Housing_DF-EVAL" 4 "" "--no-output"
    # run "bin/sum/housing/Sum_Housing_DBT-EVAL" 4 "" "--no-output"

    # for bs in 100 1000 10000 100000
    for bs in 1000
    do 
      run "bin/sum/housing/Sum_Housing_DF-INCR_ALL_${bs}" 4 "" "--no-output" ${bs}
      # run "bin/sum/housing/Sum_Housing_DF-INCR_REEVAL_ALL_${bs}" 4 "" "--no-output" ${bs}
      run "bin/sum/housing/Sum_Housing_DBT-INCR_ALL_${bs}" 4 "" "--no-output" ${bs}
    done
  done
}

####################
# SUM RETAILER
####################
function run_sum_retailer() {
  run "bin/sum/retailer/Sum_Retailer_DF-EVAL" 4 "" "--no-output"
  run "bin/sum/retailer/Sum_Retailer_DBT-EVAL" 4 "" "--no-output"

  # for bs in 100 1000 10000 100000
  for bs in 1000
  do 
    run "bin/sum/retailer/Sum_Retailer_DF-INCR_ALL_${bs}" 4 "" "--no-output" ${bs}
    run "bin/sum/retailer/Sum_Retailer_DF-INCR_INVENTORY_${bs}" 4 "" "--no-output" ${bs}
    run "bin/sum/retailer/Sum_Retailer_DF-INCR_REEVAL_ALL_${bs}" 4 "" "--no-output" ${bs}
    run "bin/sum/retailer/Sum_Retailer_DF-INCR_REEVAL_INVENTORY_${bs}" 4 "" "--no-output" ${bs}
    run "bin/sum/retailer/Sum_Retailer_DBT-INCR_ALL_${bs}" 4 "" "--no-output" ${bs}
    run "bin/sum/retailer/Sum_Retailer_DBT-INCR_INVENTORY_${bs}" 4 "" "--no-output" ${bs}
  done
}


####################
# SUM TPCH
####################
function run_sum_tpch() {
  RUNS=4
  run "bin/sum/tpch/TPCH1_DBT-EVAL" ${RUNS} "" "--no-output"
  run "bin/sum/tpch/TPCH1_DF-EVAL" ${RUNS} "" "--no-output"

  run "bin/sum/tpch/TPCH3_DBT-EVAL" ${RUNS} "" "--no-output"
  run "bin/sum/tpch/TPCH3_DF-EVAL" ${RUNS} "" "--no-output"

  run "bin/sum/tpch/TPCH5_DBT-EVAL" ${RUNS} "" "--no-output"
  run "bin/sum/tpch/TPCH5_DF-EVAL" ${RUNS} "" "--no-output"
  run "bin/sum/tpch/TPCH5_DF-EVAL_NOINDPROJ" ${RUNS} "" "--no-output"

  run "bin/sum/tpch/TPCH6_DBT-EVAL" ${RUNS} "" "--no-output"

  run "bin/sum/tpch/TPCH7_DBT-EVAL" ${RUNS} "" "--no-output"
  run "bin/sum/tpch/TPCH7_DF-EVAL" ${RUNS} "" "--no-output"
  run "bin/sum/tpch/TPCH7_DF-EVAL_NOINDPROJ" ${RUNS} "" "--no-output"

  run "bin/sum/tpch/TPCH8_DBT-EVAL" ${RUNS} "" "--no-output"
  run "bin/sum/tpch/TPCH8_DF-EVAL" ${RUNS} "" "--no-output"

  run "bin/sum/tpch/TPCH9_DBT-EVAL" ${RUNS} "" "--no-output"
  run "bin/sum/tpch/TPCH9_DF-EVAL" ${RUNS} "" "--no-output"

  run "bin/sum/tpch/TPCH10_DBT-EVAL" ${RUNS} "" "--no-output"
  run "bin/sum/tpch/TPCH10_DF-EVAL" ${RUNS} "" "--no-output"

  run "bin/sum/tpch/TPCH12_DBT-EVAL" ${RUNS} "" "--no-output"
  run "bin/sum/tpch/TPCH12_DF-EVAL" ${RUNS} "" "--no-output"

  run "bin/sum/tpch/TPCH14_DBT-EVAL" ${RUNS} "" "--no-output"
  run "bin/sum/tpch/TPCH14_DF-EVAL" ${RUNS} "" "--no-output"

  run "bin/sum/tpch/TPCH19_DBT-EVAL" ${RUNS} "" "--no-output"

  # for bs in 100 1000 10000 100000
  for bs in 1000
  do 
    run "bin/sum/tpch/TPCH1_DBT-INCR_ALL_${bs}" ${RUNS} "" "--no-output" ${bs}
    run "bin/sum/tpch/TPCH1_DF-INCR_ALL_${bs}" ${RUNS} "" "--no-output" ${bs}

    run "bin/sum/tpch/TPCH3_DBT-INCR_ALL_${bs}" ${RUNS} "" "--no-output" ${bs}
    run "bin/sum/tpch/TPCH3_DF-INCR_ALL_${bs}" ${RUNS} "" "--no-output" ${bs}

    run "bin/sum/tpch/TPCH5_DBT-INCR_ALL_${bs}" ${RUNS} "" "--no-output" ${bs}
    run "bin/sum/tpch/TPCH5_DF-INCR_ALL_${bs}" ${RUNS} "" "--no-output" ${bs}
    run "bin/sum/tpch/TPCH5_DF-INCR_ALL_NOINDPROJ_${bs}" ${RUNS} "" "--no-output" ${bs}

    run "bin/sum/tpch/TPCH6_DBT-INCR_ALL_${bs}" ${RUNS} "" "--no-output" ${bs}

    run "bin/sum/tpch/TPCH7_DBT-INCR_ALL_${bs}" ${RUNS} "" "--no-output" ${bs}
    run "bin/sum/tpch/TPCH7_DF-INCR_ALL_${bs}" ${RUNS} "" "--no-output" ${bs}
    run "bin/sum/tpch/TPCH7_DF-INCR_ALL_NOINDPROJ_${bs}" ${RUNS} "" "--no-output" ${bs}

    run "bin/sum/tpch/TPCH8_DBT-INCR_ALL_${bs}" ${RUNS} "" "--no-output" ${bs}
    run "bin/sum/tpch/TPCH8_DF-INCR_ALL_${bs}" ${RUNS} "" "--no-output" ${bs}

    run "bin/sum/tpch/TPCH9_DBT-INCR_ALL_${bs}" ${RUNS} "" "--no-output" ${bs}
    run "bin/sum/tpch/TPCH9_DF-INCR_ALL_${bs}" ${RUNS} "" "--no-output" ${bs}

    run "bin/sum/tpch/TPCH10_DBT-INCR_ALL_${bs}" ${RUNS} "" "--no-output" ${bs}
    run "bin/sum/tpch/TPCH10_DF-INCR_ALL_${bs}" ${RUNS} "" "--no-output" ${bs}

    run "bin/sum/tpch/TPCH12_DBT-INCR_ALL_${bs}" ${RUNS} "" "--no-output" ${bs}
    run "bin/sum/tpch/TPCH12_DF-INCR_ALL_${bs}" ${RUNS} "" "--no-output" ${bs}

    run "bin/sum/tpch/TPCH14_DBT-INCR_ALL_${bs}" ${RUNS} "" "--no-output" ${bs}
    run "bin/sum/tpch/TPCH14_DF-INCR_ALL_${bs}" ${RUNS} "" "--no-output" ${bs}

    run "bin/sum/tpch/TPCH19_DBT-INCR_ALL_${bs}" ${RUNS} "" "--no-output" ${bs}
  done
}

####################
# FULL JOIN HOUSING
####################
function run_fulljoin_housing() {
  # for scale in {1..20}
  for scale in 20
  do
    if [[ "$unamestr" == 'Linux' ]]; then
       sed -i "s/datasets\/housing-.*-normalised/datasets\/housing-${scale}-normalised/g" src/templates/housing_template.hpp 
    else
       sed -i '' "s/datasets\/housing-.*-normalised/datasets\/housing-${scale}-normalised/g" src/templates/housing_template.hpp 
    fi

    echo "-----------------------------------------"
    echo "SCALE: ${scale}"

    run "bin/full_join/housing/Relational_Housing_DF-EVAL" 4 "" "--no-output"
    run "bin/full_join/housing/Factorized_Housing_DF-EVAL" 4 "" "--no-output"

    # for bs in 100 1000 10000 100000
    for bs in 1000
    do 
      run "bin/full_join/housing/Relational_Housing_DF-INCR_ALL_${bs}" 4 "" "--no-output" ${bs}
      run "bin/full_join/housing/Factorized_Housing_DF-INCR_ALL_${bs}" 4 "" "--no-output" ${bs}
      run "bin/full_join/housing/Factorized_Housing_DF-REEVAL_ALL_${bs}" 4 "" "--no-output" ${bs}

      run "bin/full_join/housing/Relational_Housing_DF-INCR_ALL_${bs}" 1 "-DLOG_OUTPUT=$((500 * ${scale}))" "--no-output" ${bs}
      run "bin/full_join/housing/Factorized_Housing_DF-INCR_ALL_${bs}" 1 "-DLOG_OUTPUT=$((500 * ${scale}))" "--no-output" ${bs}
      run "bin/full_join/housing/Factorized_Housing_DF-REEVAL_ALL_${bs}" 1 "-DLOG_OUTPUT=$((500 * ${scale}))" "--no-output" ${bs}

      run "bin/full_join/housing/Relational_Housing_DF-INCR_ALL_${bs}" 1 "-ltcmalloc -DLOG_MEMORY_INFO -DLOG_OUTPUT=$((500 * ${scale}))" "--no-output" ${bs}
      run "bin/full_join/housing/Factorized_Housing_DF-INCR_ALL_${bs}" 1 "-ltcmalloc -DLOG_MEMORY_INFO -DLOG_OUTPUT=$((500 * ${scale}))" "--no-output" ${bs}
      run "bin/full_join/housing/Factorized_Housing_DF-REEVAL_ALL_${bs}" 1 "-ltcmalloc -DLOG_MEMORY_INFO -DLOG_OUTPUT=$((500 * ${scale}))" "--no-output" ${bs}
    done
  done
}

####################
# FULL JOIN RETAILER
###################
function run_fulljoin_retailer() {

  # for bs in 100 1000 10000 100000
  for bs in 1000
  do 
    run "bin/full_join/retailer/Relational_Retailer_DF-INCR_INVENTORY_IMPROVED_${bs}" 4 "" "--no-output" ${bs}
    run "bin/full_join/retailer/Factorized_Retailer_DF-INCR_INVENTORY_IMPROVED_${bs}" 4 "" "--no-output" ${bs}

    run "bin/full_join/retailer/Relational_Retailer_DF-INCR_INVENTORY_IMPROVED_${bs}" 1 "-DLOG_OUTPUT=500000" "--no-output" ${bs}
    run "bin/full_join/retailer/Factorized_Retailer_DF-INCR_INVENTORY_IMPROVED_${bs}" 1 "-DLOG_OUTPUT=500000" "--no-output" ${bs}

    run "bin/full_join/retailer/Relational_Retailer_DF-INCR_INVENTORY_IMPROVED_${bs}" 1 "-ltcmalloc -DLOG_MEMORY_INFO -DLOG_OUTPUT=500000" "--no-output" ${bs}
    run "bin/full_join/retailer/Factorized_Retailer_DF-INCR_INVENTORY_IMPROVED_${bs}" 1 "-ltcmalloc -DLOG_MEMORY_INFO -DLOG_OUTPUT=500000" "--no-output" ${bs}
  done
}

####################
# MCM
###################
function run_mcm_dbtoaster() {
  for ms in 250 500 1000 2000 4000
  do 
      echo "-----------------------------------------"
      echo "MATRIX SIZE: ${ms}"

      run "bin/mcm/MCM_DF-INCR_A2_row" 4 "-DMATRIX_SIZE=${ms} -DNUMBER_OF_UPDATES=1" "--no-output" "0"
      run "bin/mcm/MCM_DBT-INCR_A2_row" 4 "-DMATRIX_SIZE=${ms} -DNUMBER_OF_UPDATES=1" "--no-output" "0"
      run "bin/mcm/MCM_DBT-INCR_REEVAL_A2_row" 4 "-DMATRIX_SIZE=${ms} -DNUMBER_OF_UPDATES=1" "--no-output" "0"
  done

  for ms in 4000
  do 
    for rank in {1..20} 16 32 64 128 256 48 96 192
    do
      echo "-----------------------------------------"
      echo "MATRIX SIZE: ${ms}     RANK: ${rank}"

      run "bin/mcm/MCM_DF-INCR_A2_rank1" 4 "-DMATRIX_SIZE=${ms} -DNUMBER_OF_UPDATES=${rank}" "--no-output" "0"
      run "bin/mcm/MCM_DBT-INCR_A2_rank1" 4 "-DMATRIX_SIZE=${ms} -DNUMBER_OF_UPDATES=${rank}" "--no-output" "0"
      run "bin/mcm/MCM_DBT-INCR_REEVAL_A2_rank1" 4 "-DMATRIX_SIZE=${ms} -DNUMBER_OF_UPDATES=${rank}" "--no-output" "0"
    done
  done
}

function run_mcm_octave() {

  for ms in 2000 4000 8000 16000
  do
    ./src/queries/mcm/MCM_Octave_row.m ${ms} 1
  done

  for ms in 1000 2000 4000
  do
    for rank in {1..20} 16 32 64 128 256 48 96 192
    do 
    ./src/queries/mcm/MCM_Octave_rank1.m ${ms} ${rank} 
    done
  done
}

make clean
rm -f *.heap

 run_cofactor_housing
# run_cofactor_retailer
# run_cofactor_twitter
# run_cofactor_tpch

# run_sum_housing
# run_sum_retailer
# run_sum_tpch

# run_fulljoin_housing
# run_fulljoin_retailer

# run_mcm_dbtoaster
# run_mcm_octave
