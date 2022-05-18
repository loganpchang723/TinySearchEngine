# CS50 TSE
# Logan Chang, Spring 2022

## TSE Querier Design Spec

Recall the [Querier Requirements Spec](REQUIREMENTS.md); the TSE **querier** is a standalone program that reads the document files produced by the TSE crawler and index file built by the indexer, takes query from stdin and writes search results to stdout. Its companion, **fuzzquery**, generates random inputs for querier based on seedfile for testing. In this document, only the **querier** is dicussed.

### User interface

The querier's only interface with the user is on the command-line or input can be pipelined; it must always have two arguments.

```
querier pageDirectory indexFileName
```

For example:

``` bash
$ ./querier ../data/letters-3 ../data/letters-3-index
```

### Inputs and outputs

Input: the only inputs are command-line parameters; see the User Interface above.

Output: We display the search results on stdout with docID, score, and the document's URL in non-increasing score order


### Functional decomposition into modules

We anticipate the following modules or functions (along with various helper functions) which are documented in `IMPLEMENTATION.md`:

 1. *main*, which parses arguments, calls other functions to process and accumulate scores from the query, and display the search results;
 2. *validate_query*, which validates the logical structure of the query;
 3. *counters_intersect*, which calculates and stores the intersection of two counters structs;
 4. *counters_union*, which calculates and stores the union of two counters structs;
 5. *ranking_new*; which creates a new ranking struct and initializes its docID and score;
 6. *rankboard_sort*, which sorts a dynamic array of ranking structs in non-decreaing order by score;

 And some local structs:
 1. *ranking_t*, which stores a document's ID and score for the current query;
 2. *twoctrs*, which stores two pointers to counters struct to be unioned or intersected;


And some helper modules that provide data structures:

1. *bag*, a module providing a data structure and all associated functions to store a collection of data structs;
2. *counters*, a module providing a data structure and all associated functions to store an item and its count or frequency in a given context;
3. *index*, a module providing the data structure to represent the index and all associated functions to read and write index files;
4. *pagedir*, a module providing all associated functions to determine if pageDirectory was created by the crawler and get webpages from the pageDirectory;
5. *file*, a module providing all associated functions to read and write to and from files;
6. *word*, a module providing a function to normalize a word or query;
 

### Pseudo code for logic/algorithmic flow

The querier will run as follows:

For `int main`:
1. Read in and validate the command-line arguments (ie. make sure there are 2 arguments, `pageDirectory`is the pathname for an existing directory and was produced by the crawler, `indexFileName` is the pathname of a writeable file)
2. Load the `index` struct from `indexFileName` 
3. Display the user-input prompt and begin to read queries
	4. Normalize the query and tokenize it by whitespace
	5. Check each token (which reprensents a word) for non-alphabetic and non-space characters, and print an error message and prompt a new query if a non-alphabetic and non-space character is seen
	6. Call `validate_query` to ensure the query has a valid logical structure
	7. Print the query to stdout
	8. Initialize a `counters` struct to keep track of the docIDs and counts of the current andsequence and a `bag` struct to hold the `counters` struct of each andsequence
		9. Update the docIDs and counts of the current andsequence by taking the intersection of the current andsequence and the `counters` struct of the current word from the `index` struct
	10. Calculate the docIDs and counts matching the query by unioning each andsequence in the `bag` struct holding all andsequence docIDs and counts
	11. Find the number of unique docIDs in the union of the query and create an array of `ranking` structs for each docID and score of matching length
	12. Insert the `ranking` objects into the array and sort them by non-increasing score
	13. Print each docID, score, and url of the document that match the query in non-increasing score order
	14. Free all associated dynamic memory
	15. Prompt the user for a new query, stopping when the user inputs the kill command (ctrl-D)

For `bool validate_query`:
Arguments: `words` (a pointer to a pointer of string [dynamic string array] representing the query) and `query_len` (an int)
1. Return false if words starts or ends with 'and' or 'or'
2. Return false if 'and' or 'or' appear in consecutive positions in the query
3. Return true if neither case is caught, thus making the query logically valid

For `void counters_intersect`:
Arguments: two pointers to `counters` struct to be intersected
1. Store the intersection in the first `counters` pointer by taking the minimum of each `counters` structs count for matching keys

For `void counters_union`:
Arguments: two pointers to `counters` struct to be union
1. Store the union in the first `counters` pointer by taking the sum of each `counters` structs count for corresponding keys

For `static void rankboard_sort`:
Arguments: `rankboard` (a pointer to an array of `ranking` structs) and `numDocs` (an int representing the length of `rankboard`)
1. Sorts `rankboard` in place by non-increasin score order via insertion sort


### Dataflow through modules

1. *main*, which parses parameters, passes the query to *validate_query*, passes *counters* for andsequences and the total query to *counters_intersect* and *counters_union*, creates a dynamic array of *ranking* structs, and prints the query's results;
2. *validate_query*, which takes the query and validates its logical structure;
3. *counters_intersect*, which calculates and stores the intersection of two counters structs;
4. *counters_union*, which calculates and stores the union of two counters structs;
5. *ranking_new*; which creates a new ranking struct and initializes its docID and score;
6. *rankboard_sort*, which takes a dyanamic array of *ranking* structs and sorts them in non-increasing order by score.


### Major data structures

The querier uses a `ranking` data structure that stores a document's ID and its score for the given query. The querier also uses a local `twocts` struct which simply holds two `counters` struct pointers

### Testing plan

*Unit testing*. Given that there is only one main function and no functional auxiliary/helper modules for querier, unit testing will primarily consist of testing the parsing of different commandline arguments.

*Integration testing*.  Assemble the querier and test it as a whole.
The *querier* will be fuzz tested by using *fuzzquery* and utilizing verified crawler directories and index files from `/thayerfs/courses/22spring/cosc050/cs50tse/tse-output`.

0. Test the program with various forms of incorrect command-line arguments to ensure that its command-line parsing, and validation of those parameters, works correctly.

0. Test the querier with invalid queries consisting of non-alphabetic and non-space characters.

0. Test the querier with invalid queries consisting of non-logical queries (ie. starting or ending with 'and' or 'or', consecutive 'and' or 'or).

1. Test the querier on a simple query with various capitalization and spacing to ensure the query is parsed and normalized correctly.

2. Test the querier with several fuzz tests using *fuzzquery* and different crawler directories and index files and study the results.

3. Run *valgrind* on a different fuzz test to ensure the querier has no memory leaks or errors.