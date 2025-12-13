#!/usr/bin/env bash
set -euo pipefail

# --------------------------------------------------
# Usage
# --------------------------------------------------

usage() {
  cat <<EOF
Usage:
  $(basename "$0") <generated_include.hpp> [executable_name]

Arguments:
  generated_include.hpp   Mandatory generated C++ include file
  executable_name         Optional output executable (default: a.out)

Example:
  ./build-generated-cpp.sh generated.hpp my_app
  ./build-generated-cpp.sh generated.hpp
EOF
  exit 1
}

# --------------------------------------------------
# Argument parsing
# --------------------------------------------------

if [[ $# -lt 1 || $# -gt 2 ]]; then
  usage
fi

GENERATED_INCLUDE="$1"
EXECUTABLE="${2:-a.out}"

# --------------------------------------------------
# Paths
# --------------------------------------------------

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
RUNTIME_DIR="$ROOT_DIR/runtime"

# --------------------------------------------------
# Validation
# --------------------------------------------------

if [[ ! -f "$GENERATED_INCLUDE" ]]; then
  echo "Error: Generated include file not found: $GENERATED_INCLUDE"
  exit 1
fi

# --------------------------------------------------
# Compiler settings
# --------------------------------------------------

CXX=${CXX:-g++}
CXXFLAGS="-O3 -DNDEBUG -Wall -Wno-unused-variable -std=c++17 -pedantic"
INCLUDES="-I ${RUNTIME_DIR}/include -I ${RUNTIME_DIR}/include/dbtoaster"

APP_INCLUDE=${APP_INCLUDE:-"$RUNTIME_DIR/include/application_base.hpp"}
EXTRA_ARGS=${EXTRA_ARGS:-}

# --------------------------------------------------
# Build
# --------------------------------------------------

RUNTIME_SOURCES=$(find "$RUNTIME_DIR/src" -name "*.cpp")

echo "==> Compiling generated C++"
echo "    Generated include : $GENERATED_INCLUDE"
echo "    Output executable : $EXECUTABLE"
echo "    Command :" $CXX \
  $CXXFLAGS \
  $INCLUDES \
  $EXTRA_ARGS \
  $RUNTIME_DIR/src/main.cpp \
  -include "$GENERATED_INCLUDE" \
  -include $APP_INCLUDE \
  -o "$EXECUTABLE"

$CXX \
  $CXXFLAGS \
  $INCLUDES \
  $EXTRA_ARGS \
  $RUNTIME_DIR/src/main.cpp \
  -include "$GENERATED_INCLUDE" \
  -include $APP_INCLUDE \
  -o "$EXECUTABLE"

# --------------------------------------------------
# Done
# --------------------------------------------------

echo "==> Build successful"
echo "Binary: $EXECUTABLE"
