# CS50 TSE 

## Crawler Module

This is the directory with all of the relevant files needed to support and implement the *crawler* module of the Tiny Search Engine. The implmentation follows the specs layed out in `DESIGN.md` and `REQUIREMENTS.md`.

### Assumptions
Beyond the specs and assumptions in `REQUIREMENTS.md`, the crawler will print an error message and exit non-zero if the provided `seedURL` is non-internal. 

### Compilation
To compile the *crawler* module, simply run `make` in the `crawler` directory or run `make` in the parent directory to compile all modules.

### Testing
To run the suite of test cases specified in `testing.sh`, simply run `make test` in the `crawler` directory, which will run the suite of test cases in `testing.sh` and provided all stderr and stdout output from these test cases to `testing.out` and create all output files made by the crawler in the `../data/` directory. 

To run `valgrind` on the crawler, simply run `make valgrind`, which will run valgrind on a pre-selected test case.

*Note: running `make test` will run `valgrind` on a pre-selected test case that differs from `make valgrind`. Running `make valgrind` prior to `make test` will cause an error since the directory where `make valgrind` saves its files is created during `make test`, so please run `make test` prior to `make valgrind` (this will run two seperate valgrind tests)*