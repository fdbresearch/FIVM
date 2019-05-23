#!/bin/bash

BIN_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
FRONTEND_DIR=${BIN_DIR}/../frontend

echo "Compiling F-IVM frontend..."
cd ${FRONTEND_DIR}
sbt assembly
cd -
echo "Done."