# CS50 TSE

## Indexer Implementation

### Indexer API

#### index_build
Loops through each webpage file in pageDir and builds the inverted index for the directory by calling `index_page` on each webpage file and writes the completed index to the file pointer fp
```c 
static void index_build (char* pageDir, FILE* fp)
```
#### index_page
Goes through each word on the webpage and counts the number of occurrences of each unique word longer than 3 characters and stores the results in index
```c 
bool index_page (index_t* index, webpage_t* page, const int docID)
```

### Pseudocode

The indexer will run as follows:

For `int main`:
1. Read in and validate the command-line arguments (ie. make sure there are 2 arguments, `pageDirectory`is the pathname for an existing directory and was produced by the crawler, `indexFileName` is the pathname of a writeable file)
2. Create a pointer, `fp`, to `indexFileName` 
3. Call `index_build` with `pageDirectory` and `fp` arguments

For `void index_build`:
Arguments: `pageDir` (a string) and `fp` (a pointer to the file named `indexFileName`)
1. Create a new *index* object
2. Starting from 1, loop over all document IDs in `pageDirectory`,
	3. Create a *webpage* object from the file *pageDirectory/documentID*
	4. If the webpage is created,
		5. Add the webpage and its words to the *index* object by calling *index_page*
6. Save the *index* object to `indexFileName`
7. Delete the *index* object from memory

For `bool index_build`:
Arguments: `index` (a pointer to an *index* struct), `page` (a pointer to a *webpage* struct), and `docID` (an integer)
1. While there are words in the *webpage* object,
	2. Ignore words of length less than 3
	3. Normalize the word (convert to all lowercase)
	4. Insert the word into the *index* object by,
		5. If the word is not already in the *index* object,
			6. Initialize a *counters* object for that word
		7. Else, return false
		8. Increment the *counters* object for that word
9. All words have been processed and stored in the index, so return true

### Data Structures
The key data structure is the *index*, which maps each unqiue word longer than 3 characters in `pageDirectory` to (*docID*, *#occurrences*) pairs. The *index* is a *hashtable*, keyed by word and storing *counters* structs as items. The *counters* struct is keyed by *docID* and stores a count of the number of occurrences of that word in the document named "*pageDirectory/docID*".

### Other Modules
The indexer leverages other modules in the `common` and `libcs50` directories. In particular, the indexer utilizes the `pageDir`, `index`, and `word` mdouels in the `common` directory. The `index` module is built off of the `hashtable`, `set`, and `counters` modules in `libcs50`. The documentation of each of these modules can be found in their respective header files.

### Testing plan

*Unit testing*.  The *indextest* program is the unit test for the indexer module. It reads the index file and writes the in-memory *index* object from that file to a new file.

*Integration testing*.  Assemble the indexer and test it as a whole.
The *indexer* will be tested by building an index from a pageDirectory created by the crawler. The resulting index will be validated by using *indextest* to ensure it can be loaded, read, and written to a new index file.

0. Test the program with various forms of incorrect command-line arguments to ensure that its command-line parsing, and validation of those parameters, works correctly.

0. Test the indexer with a faulty `pageDirectory`.

0. Test the indexer with a faulty `indexFileName` (including a directory as `indexFileName`).

0. Test the indexer with a `pageDirectory` not created by the crawler.

0. Test the indexer with a `indexFileName` that is read-only.

1. Test the indexer on several different directories created by the crawler. Use *indextest* to validate the *index* object can be loaded, read, and written. Compare the original index file and the index file created by *indextest* to ensure the *index* objects written to both files have identical contents.

2. Run *valgrind* on both the *indexer* and *indextest* to ensure both programs have no memory leaks or errors.