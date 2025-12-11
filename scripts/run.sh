#!/bin/bash

FIVM_VERSION=1.0
SCALA_VERSION=2.12

SCRIPTS_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
COMPILER_DIR=${SCRIPTS_DIR}/../compiler
COMPILER_BIN=${COMPILER_DIR}/target/scala-${SCALA_VERSION}/FIVM-assembly-${FIVM_VERSION}.jar

if [ ! -f ${COMPILER_BIN} ]
then
    ${SCRIPTS_DIR}/compile.sh
fi

scala $COMPILER_BIN $@