# CS50 TSE

## TSE Indexer Design Spec

Recall the [Indexer Requirements Spec](REQUIREMENTS.md); the TSE **indexer** is a stand-alone program that reads the document files produced by the TSE crawler, builds an index, and writes that index to a file. Its companion, the **index tester**, loads an index file produced by the indexer and saves it to another file. In this document, only the **indexer** is discussed.

### User interface

The indexer's only interface with the user is on the command-line; it must always have two arguments.

```
indexer pageDirectory indexFileName
```

For example:

``` bash
$ ./crawler ../data/letters-3 ../data/letters-3-index
```

### Inputs and outputs

Input: the only inputs are command-line parameters; see the User Interface above.

Output: We save the index to `indexFileName` using the format described in the Requirements


### Functional decomposition into modules

We anticipate the following modules or functions:

 1. *main*, which parses arguments and initializes other modules;
 2. *build_index*, which builds the index and loops over all pages in `pageDirectory` sequentially;
 3. *index_page*, which indexes a webpage and adds each word and its associated count to the index.


And some helper modules that provide data structures:

1. *index*, a module providing the data structure to represent the index and all associated functions to read and write index files;
2. *webpage*, a module providing the data structure to represent webpage and all associated functions to scan a webpage for words;
3. *pagedir*, a module providing all associated functions to determine if pageDirectory was created by the crawler and get webpages from the pageDirectory;
4. *word*, a module providing a function to normalize a word.

### Pseudo code for logic/algorithmic flow

The indexer will run as follows:

1. Execute from a command line as shown in the User Interface section
2. Parse the command line, validate parameters, initialize other modules
3. Build the index in memory by loading data from `pageDirectory` by calling *index_build*

*index_build* will run as follows:

1. Create a new *index* object
2. Starting from 1, loop over all document IDs in `pageDirectory`,
	3. Create a *webpage* object from the file *pageDirectory/documentID*
	4. If the webpage is created,
		5. Add the webpage and its words to the *index* object by calling *index_page*
6. Save the *index* object to `indexFileName`
7. Delete the *index* object from memory

*index_page* will run as follows:
1. While there are words in the *webpage* object,
	2. Ignore words of length less than 3
	3. Normalize the word (convert to all lowercase)
	4. Insert the word into the *index* object by,
		5. If the word is not already in the *index* object,
			6. Initialize a *counters* object for that word
		7. Else, return false
		8. Increment the *counters* object for that word
9. All words have been processed and stored in the index, so return true


### Dataflow through modules

 1. *main* parses parameters and passes them to *index_build*
 2. *index_build* creates a new *index* object for all of the files in `pageDirectory` to be saved to the provided `indexFileName`
 3. *index_page* takes the *index* object, reads through each word in the HTML of the *webpage* object from `pageDirectory` of form "*pageDirectory/docID*", and adds each words frequency to the *index* object


### Major data structures

The key data structure is the *index*, which maps each unqiue word longer than 3 characters in `pageDirectory` to (*docID*, *#occurrences*) pairs. The *index* is a *hashtable*, keyed by word and storing *counters* structs as items. The *counters* struct is keyed by *docID* and stores a count of the number of occurrences of that word in the document named "*pageDirectory/docID*".

### Testing plan

*Unit testing*.  The *indextest* program is the unit test for the indexer module. It reads the index file and writes the in-memory *index* object from that file to a new file.

*Integration testing*.  Assemble the indexer and test it as a whole.
The *indexer* will be tested by building an index from a pageDirectory created by the crawler. The resulting index will be validated by using *indextest* to ensure it can be loaded, read, and written to a new index file.

0. Test the program with various forms of incorrect command-line arguments to ensure that its command-line parsing, and validation of those parameters, works correctly.

0. Test the indexer with a faulty `pageDirectory`.

0. Test the indexer with a faulty `indexFileName` (including a directory as `indexFileName`).

0. Test the indexer with a `pageDirectory` not created by the crawler.

0. Test the indexer with a `indexFileName` that is read-only.

1. Test the indexer on several different directories created by the crawler. Use *indextest* to validate the *index* object can be loaded, read, and written. Compare the original index file with the index file in `/thayerfs/courses/22spring/cosc050/cs50tse/tse-output/` to check its correctness and with the index file created by *indextest* to ensure the *index* objects written to both files have identical contents.

2. Run *valgrind* on both the *indexer* and *indextest* to ensure both programs have no memory leaks or errors.