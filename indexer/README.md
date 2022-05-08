# CS50 TSE 

## Indexer Module

This is the directory with all of the relevant files needed to support and implement the *indexer* module of the Tiny Search Engine. The implmentation follows the specs layed out in `DESIGN.md` and `REQUIREMENTS.md`.

### Assumptions
Beyond the specs and assumptions in `REQUIREMENTS.md`, the indexer makes no additional assumptions. 

### Compilation
To compile the *indexer* module, simply run `make` in the `indexer` directory or run `make` in the parent directory to compile all modules.

### Testing
To run the suite of test cases specified in `testing.sh`, simply run `make test` in the `indexer` directory, which will run the suite of test cases in `testing.sh` and provided all stderr and stdout output from these test cases to `testing.out` and create all output files made by the indexer in the `../data/` directory. These include a suite of test cases run with `valgrind` that differ from the testcases run with `make valgrind` (*see below*).

To run `valgrind` on the indexer, simply run `make valgrind`, which will run valgrind on pre-selected test cases.