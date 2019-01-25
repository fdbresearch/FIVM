#!/bin/bash

FIVM_VERSION=1.0
SCALA_VERSION=2.12

BIN_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
FRONTEND_DIR=${BIN_DIR}/../frontend
FRONTEND_BIN=${FRONTEND_DIR}/target/scala-${SCALA_VERSION}/FIVM-assembly-${FIVM_VERSION}.jar

if [ ! -f ${FRONTEND_BIN} ]
then
    ${BIN_DIR}/compile_frontend.sh
fi

scala $FRONTEND_BIN $@