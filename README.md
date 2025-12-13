# F-IVM

This repository contains the implementation of Factorized Incremental View Maintenance (F-IVM) based on the SIGMOD 2018 paper: [Incremental View Maintenance with Triple Lock Factorization Benefits](http://www.cs.ox.ac.uk/dan.olteanu/papers/no-sigmod18.pdf). 

F-IVM leverages higher-order IVM to reduce the maintenance of the input view to the maintenance of a tree of increasingly simpler views. F-IVM takes a SQL query and a variable order (tree decomposition) for that query as input and produces C++ code optimized for processing batch or single-tuple updates to input relations.

The frontend of F-IVM produces high-level update trigger specifications for the input query. Its backend builds on the [DBToaster](https://dbtoaster.github.io/) code generator by adding support for custom ring types, enabling transformation of these specifications into native code. 

## Installation 

The `compiler` directory contains the Scala implementation of F-IVM. To compile the entire framework run:
```
scripts/build-compiler.sh
```

The `runtime` directory contains a C++ driver application and the runtime library header files needed to compile the generated C++ code. 

Requirements: Scala 2.12, JDK 17 (older versions might also work), and g++ 6.3.0 or later.


## Compiling SQL queries to C++ code

F-IVM compiles SQL queries into trigger programs expressed in DBToaster's intermediate language, called M3, which are subsequently compiled into C++ code. 

```
> scripts/generate-code.sh
F-IVM 1.1
Usage: sbt run [options] <SQL file>

  <SQL file>           Input SQL file
  -o, --output <file>  Output file
  -l, --lang <cpp|m3>  Specify output language: cpp or m3 (default: cpp)
  --help               prints this usage text
```
The `examples` directory contains example queries and datasets. 

To compile a SQL query into C++ code run:
```
scripts/generate-code.sh -l cpp examples/queries/housing/housing_regression.sql -o housing_cofactor.hpp
```
The input file `housing_regression.sql` contains a SQL query, a variable order for the query, and a ring type definition. The output file contains trigger functions and supporting data structures, and includes files from the runtime library in `runtime/include`. The generated code can be embedded into users' applications. 

To obtain M3 code with high-level definitions of the trigger programs run:
```
scripts/generate-code.sh -l m3 examples/queries/housing/housing_regression.sql -o housing_regression.m3
```

Note: The supported SQL syntax is fairly rudimentary. F-IVM currently supports only group-by SUM aggregates over the natural join of input relations. The SUM aggregate, however, can be defined over an arbitrary ring of values. See some example queries and the [SIGMOD 2018 paper](http://www.cs.ox.ac.uk/dan.olteanu/papers/no-sigmod18.pdf) for more details.

## Running example queries

The `examples` directory provides example queries using custom payload (aggregate) types, type definitions, variable orders, toy datasets, and example applications. The `build-examples.sh` script compiles all the SQL files from `examples/queries`.
```
cd examples
build-examples.sh
bin/housing/housing_regression
```

## Custom rings

F-IVM supports SQL aggregate queries where the aggregate values are from an arbitrary ring. The `example` directory provides several examples of different rings:

* the rings used for gradient computation in learning regression models 
    * the ring definitions are in `examples/include/ring/ring_cofactor_degree1.hpp` and `examples/include/ring/ring_cofactor_degree2.hpp`
    * e.g., `examples/queries/housing/housing_regression.sql`.

* the ring of relational aggregates that can capture the listing representation of a join result 
    * the ring definition is in `examples/include/ring/ring_relational_opt.hpp`
    * e.g., `examples/queries/housing/housing_listing_join.sql`.

* the ring of relational aggregates that can capture the factorized representation of a join result 
    * the ring definition is in `examples/include/ring/ring_factorized.hpp`
    * e.g., `examples/queries/housing/housing_factorized_join.sql`.