#!/bin/bash

BIN_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
BACKEND_DIR=${BIN_DIR}/../backend
BACKEND_BIN=${BACKEND_DIR}/dbtoaster/bin/dbtoaster
BACKEND_FLAGS="-l cpp -O3 -xhashmap -xruntime -F HEURISTICS-DECOMPOSE-OVER-TABLES"

${BACKEND_BIN} ${BACKEND_FLAGS} $@
