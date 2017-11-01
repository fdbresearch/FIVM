# F-IVM

This repository contains the code, datasets, and scripts for the experiments from the F-IVM paper. The `src/queries` directory contains the queries in SQL, M3 (DBToaster's intermediate language), Octave, or C++ format.

The `release` directory contains the latest DBToaster binary for MacOS. For Linux, download the latest binary for Linux from www.dbtoaster.org, and copy `bin/dbtoaster_release` from the unzipped directory to `release/bin/dbtoaster_frontend` in this repository.

## Installation

Running the experiments requires Scala 2.11, JDK 1.8 (JDK 1.6 and 1.7 might also work), and g++ 6.3 or later.

Note: g++ 4.8.4 has compilation bugs, install g++ 6.3.0 or later.

## Running experiments
```sh
./run_experiments.sh

```

Modify `run_experiments.sh` to select a subset of experiments.

Choose a different dataset size for the experiments by modifying the corresponding template file of the dataset in `src/template`.
