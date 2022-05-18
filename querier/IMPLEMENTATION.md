# CS50 TSE
# Logan Chang, Spring 2022

## Querier Implementation

### Querier API

#### prompt
Loads the prompt onto stdout for the user to enter a query
```c
static void prompt(void); 
```
#### validate_query
Validates the logical structure of the query. Returns true if the query is logically sound, else return false
```c
bool validate_query(char** array, const int query_len);
```
#### counters_intersect
Calculates the intersection between two counters where each counter holds (docID, count) pairs
```c
void counters_intersect(counters_t* ct1, counters_t* ct2);
```
#### counters_union
Calculates the union between two counters where each counter holds (docID, count) pairs
```c
void counters_union(counters_t* ct1, counters_t* ct2);
```
#### ranking_new
Creates a pointer to a new ranking struct with key representing docID and count representing score/ranking
```c
ranking_t* ranking_new(int key, int count); 
```
#### rankboard_sort
Sorts a pointer to an array of ranking structs that is of numDocs length
```c
static void rankboard_sort(ranking_t** rankBoard, int numDocs);
```

### Pseudocode

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

### Data Structures
The querier uses a `ranking` data structure that stores a document's ID and its score for the given query. The querier also uses a local `twocts` struct which simply holds two `counters` struct pointers

### Other Modules
The querier leverages other modules in the `common` and `libcs50` directories. In particular, the querier utilizes the `pageDir`, `index`, `file`, and `word` modules in the `common` directory along with the `counters`, and `bag` modules in `libcs50`. The documentation of each of these modules can be found in their respective header files.

### Testing plan

*Unit testing*. Given that there is only one main function and no auxiliary modules for querier, unit testing will primarily consist of testing the parsing of different commandline arguments.

*Integration testing*.  Assemble the querier and test it as a whole.
The *querier* will be fuzz tested by using *fuzzquery* and utilizing verified crawler directories and index files from `/thayerfs/courses/22spring/cosc050/cs50tse/tse-output`.

0. Test the program with various forms of incorrect command-line arguments to ensure that its command-line parsing, and validation of those parameters, works correctly.

0. Test the querier with invalid queries consisting of non-alphabetic and non-space characters.

0. Test the querier with invalid queries consisting of non-logical queries (ie. starting or ending with 'and' or 'or', consecutive 'and' or 'or).

1. Test the querier on a simple query with various capitalization and spacing to ensure the query is parsed and normalized correctly.

2. Test the querier with several fuzz tests using *fuzzquery* and different crawler directories and index files and study the results.

3. Run *valgrind* on a different fuzz test to ensure the querier has no memory leaks or errors.