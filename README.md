# F-IVM

This repository contains the implementation of Factorized Incremental View Maintenance (F-IVM) based on the SIGMOD 2018 paper: [Incremental View Maintenance with Triple Lock Factorization Benefits](http://www.cs.ox.ac.uk/dan.olteanu/papers/no-sigmod18.pdf). 

F-IVM leverages higher-order IVM to reduce the maintenance of the input view to the maintenance of a tree of increasingly simpler views. F-IVM takes a SQL query and a variable order (tree decomposition) for that query as input and produces C++ code optimized for processing batch or single-tuple updates to input relations.

The frontend of F-IVM produces high-level update trigger specifications for the input query. The backend of F-IVM relies on [DBToaster](https://dbtoaster.github.io/), which is extended to support custom ring types, to transform these trigger specifications into native code. 

## Installation 

The `frontend` directory contains the Scala implementation of F-IVM. To compile the frontend just run
```
bin/compile_frontend.sh
```

The `backend` directory contains precompiled DBToaster binaries for MacOS and Linux, and header files of the runtime library needed to compile with generated C++ code. 

Requirements: Scala 2.12, JDK 1.8 (older versions might also work), and g++ 6.3.0 or later (noticed compilation bugs with g++ 4.8.4 on MacOS).


## Compiling SQL queries to M3 programs

The F-IVM frontend compiles SQL queries into trigger programs expressed using DBToaster's intermediate language called M3.

```
> bin/run_frontend.sh
F-IVM 1.0
Usage: sbt run [options] <SQL file>

  <SQL file>              Input SQL file
  -o, --output <M3 file>  Output M3 file
  --batch                 Generate batch update triggers
  --single                Generate single-tuple update triggers
  --help                  prints this usage text
```
The `examples` directory contains example queries and datasets. 
```
> bin/run_frontend.sh examples/queries/housing/housing_cofactor.sql -o housing_cofactor.m3
```
The input file `housing_cofactor.sql` contains a SQL query, a variable order for the query, and a ring type definition. The output file `housing_cofactor.m3` contains M3 code with high-level definitions of trigger programs.

Note: The supported SQL syntax is fairly rudimentary. F-IVM currently supports only group-by SUM aggregates over the natural join of input relations. The SUM aggregate, however, can be defined over an arbitrary ring of values. See some example queries and the [SIGMOD 2018 paper](http://www.cs.ox.ac.uk/dan.olteanu/papers/no-sigmod18.pdf) for more details.


## Compiling M3 programs to C++ code

To compile generated M3 programs into C++ code run:
```
bin/run_backend.sh housing_cofactor.m3 -o housing_cofactor.hpp
```
The generated code defines trigger functions and supporting data structures, and includes files from the runtime library in `backend/lib`. The generated code can be embedded into users' applications. 


## Running example queries

The `examples` directory provides example queries using custom payload (aggregate) types, type definitions, variable orders, toy datasets, and example applications. The `makefile` script compiles all the SQL files from `examples/queries`.
```
cd examples
make
bin/housing/housing_cofactor_BATCH_1000
```

## Custom rings

F-IVM supports SQL aggregate queries where the aggregate values are from an arbitrary ring. The `example` directory provides several examples of different rings:

* the rings used for gradient computation in learning regression models 
    * the ring definitions are in `examples/code/ring/ring_cofactor_degree1.hpp` and `examples/code/ring/ring_cofactor_degree2.hpp`
    * e.g., `examples/queries/housing/housing_cofactor.sql`.

* the ring of relational aggregates that can capture the listing representation of a join result 
    * the ring definition is in `examples/code/ring/ring_relational_opt.hpp`
    * e.g., `examples/queries/housing/housing_listing_join.sql`.

* the ring of relational aggregates that can capture the factorized representation of a join result 
    * the ring definition is in `examples/code/ring/ring_factorized.hpp`
    * e.g., `examples/queries/housing/housing_factorized_join.sql`.