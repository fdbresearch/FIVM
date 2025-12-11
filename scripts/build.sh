#!/bin/bash

SCRIPTS_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
COMPILER_DIR=${SCRIPTS_DIR}/../compiler

echo "Compiling F-IVM..."
cd ${COMPILER_DIR}
sbt assembly
cd -
echo "Done."
