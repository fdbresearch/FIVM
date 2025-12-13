#!/bin/bash

make clean
make

mkdir -p bin/favorita
mkdir -p bin/housing
mkdir -p bin/retailer
mkdir -p bin/tpch

BUILD_SCRIPT=../scripts/build-generated-cpp.sh

# --------------------------------------------------
# Linear regression examples
# --------------------------------------------------

APP_INCLUDE=include/application/favorita/application_favorita_regression.hpp \
EXTRA_ARGS="-I include" \
${BUILD_SCRIPT} generated/cpp/favorita/favorita_regression_continuous.hpp bin/favorita/favorita_regression_continuous_app

APP_INCLUDE=include/application/housing/application_housing_regression.hpp \
EXTRA_ARGS="-I include" \
${BUILD_SCRIPT} generated/cpp/housing/housing_regression.hpp bin/housing/housing_regression_app

APP_INCLUDE=include/application/retailer/application_retailer_regression.hpp \
EXTRA_ARGS="-I include" \
${BUILD_SCRIPT} generated/cpp/retailer/retailer_regression.hpp bin/retailer/retailer_regression_app


# --------------------------------------------------
# Listing representation examples
# --------------------------------------------------

APP_INCLUDE=include/application/housing/application_housing_listing_join.hpp \
EXTRA_ARGS="-I include" \
${BUILD_SCRIPT} generated/cpp/housing/housing_listing_join.hpp bin/housing/housing_listing_join_app

APP_INCLUDE=include/application/retailer/application_retailer_listing_join.hpp \
EXTRA_ARGS="-I include" \
${BUILD_SCRIPT} generated/cpp/retailer/retailer_listing_join.hpp bin/retailer/retailer_listing_join_app

APP_INCLUDE=include/application/tpch/application_tpch_FQ1_listing_join.hpp \
EXTRA_ARGS="-I include" \
${BUILD_SCRIPT} generated/cpp/tpch/tpch_FQ1_listing_join.hpp bin/tpch/tpch_FQ1_listing_join_app

APP_INCLUDE=include/application/tpch/application_tpch_FQ2_listing_join.hpp \
EXTRA_ARGS="-I include" \
${BUILD_SCRIPT} generated/cpp/tpch/tpch_FQ2_listing_join.hpp bin/tpch/tpch_FQ2_listing_join_app

APP_INCLUDE=include/application/tpch/application_tpch_FQ3_listing_join.hpp \
EXTRA_ARGS="-I include" \
${BUILD_SCRIPT} generated/cpp/tpch/tpch_FQ3_listing_join.hpp bin/tpch/tpch_FQ3_listing_join_app

APP_INCLUDE=include/application/tpch/application_tpch_FQ4_listing_join.hpp \
EXTRA_ARGS="-I include" \
${BUILD_SCRIPT} generated/cpp/tpch/tpch_FQ4_listing_join.hpp bin/tpch/tpch_FQ4_listing_join_app

# --------------------------------------------------
# Factorized representation examples
# --------------------------------------------------

APP_INCLUDE=include/application/housing/application_housing_factorized_join.hpp \
EXTRA_ARGS="-I include" \
${BUILD_SCRIPT} generated/cpp/housing/housing_factorized_join.hpp bin/housing/housing_factorized_join_app

APP_INCLUDE=include/application/retailer/application_retailer_factorized_join.hpp \
EXTRA_ARGS="-I include" \
${BUILD_SCRIPT} generated/cpp/retailer/retailer_factorized_join.hpp bin/retailer/retailer_factorized_join_app

APP_INCLUDE=include/application/tpch/application_tpch_FQ1_factorized_join.hpp \
EXTRA_ARGS="-I include" \
${BUILD_SCRIPT} generated/cpp/tpch/tpch_FQ1_factorized_join.hpp bin/tpch/tpch_FQ1_factorized_join_app

APP_INCLUDE=include/application/tpch/application_tpch_FQ2_factorized_join.hpp \
EXTRA_ARGS="-I include" \
${BUILD_SCRIPT} generated/cpp/tpch/tpch_FQ2_factorized_join.hpp bin/tpch/tpch_FQ2_factorized_join_app

APP_INCLUDE=include/application/tpch/application_tpch_FQ3_factorized_join.hpp \
EXTRA_ARGS="-I include" \
${BUILD_SCRIPT} generated/cpp/tpch/tpch_FQ3_factorized_join.hpp bin/tpch/tpch_FQ3_factorized_join_app

APP_INCLUDE=include/application/tpch/application_tpch_FQ4_factorized_join.hpp \
EXTRA_ARGS="-I include" \
${BUILD_SCRIPT} generated/cpp/tpch/tpch_FQ4_factorized_join.hpp bin/tpch/tpch_FQ4_factorized_join_app
