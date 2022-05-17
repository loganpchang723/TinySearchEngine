# CS50 TSE 
# Logan Chang
# Spring 2022

## Querier Module

This is the directory with all of the relevant files needed to support and implement the *querier* module of the Tiny Search Engine. The implmentation follows the specs layed out in `DESIGN.md` and `REQUIREMENTS.md`.

### Assumptions
Beyond the specs and assumptions in `REQUIREMENTS.md`, the querier makes no additional assumptions.

### Compilation
To compile the *crawler* module, simply run `make` in the `querier` directory or run `make` in the parent directory to compile all modules.

### Testing
To run the suite of test cases specified in `testing.sh`, simply run `make test` in the `querier` directory, which will run the suite of test cases in `testing.sh` and provided all stderr and stdout output from these test cases to `testing.out`. 

To run `valgrind` on the querier, simply run `make valgrind`, which will run valgrind on a pre-selected test case.