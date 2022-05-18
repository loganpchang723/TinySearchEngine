/* 
 * querier.c - CS50 'querier' for TSE
 *
 *
 * Logan Chang, 5/18/22
 * CS50, Spring 2022
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "index.h"
#include "pagedir.h"
#include "word.h"
#include "bag.h"
#include "counters.h"
#include "file.h"
#include "ctype.h"

/******** local data types *******/
struct twocts {
	counters_t *result;
	counters_t *another;
};

typedef struct ranking {
    int docID;
    int score;
} ranking_t;

/**************function prototypes*************/

/****************querier functions****************/
int fileno(FILE *stream);
static void prompt(void); 
bool validate_query(char** array, const int query_len);
void counters_intersect(counters_t* ct1, counters_t* ct2);
void counters_union(counters_t* ct1, counters_t* ct2);
ranking_t* ranking_new(int key, int count); 
static void rankarray_sort(ranking_t** rankarray, int numDocs);
/************************************************/

/****************iterator functions****************/
void intersect_helper(void *arg, const int key, const int count);
void union_helper(void *arg, const int key, const int count);
void count_docs(void *arg, const int key, const int count);
void counters_insert(void *arg, const int key, const int count);
/*************************************************/

/****************helper functions****************/
static int min(const int x, const int y);
void itemdelete(void* item);
/************************************************/

/*************************************************/

int main(const int argc, char* argv[])
{
    // validate command-line arguments
    if (argc == 3){
        // parse the arguments
        char* pageDir = argv[1];
        char* indexFileName = argv[2];

        // check if arguments fit all criteria, otherwise exit non-zero
        if (!is_crawler_dir(pageDir)){
            fprintf(stderr, "ERROR: pageDir was not produced by crawler\n");
            return 1;
        }
        FILE* fp;
        fp = fopen(indexFileName, "r");
        if (fp == NULL){
            fprintf(stderr, "ERROR: index file cannot be read\n");
            return 1;
        }

        // load index from indexFileName
        index_t* index = index_load(fp);
        fclose(fp);
        if (index == NULL){
            fprintf(stderr, "ERROR: index could not be loaded from index file\n");
            return 1;
        }

        // read queries until the user exits
        prompt();
        char* query;
        char* normalQuery;
        // char** words;

        while ( (query = freadlinep(stdin)) != NULL){
            // parse the query into an array of words
            int max_words = strlen(query) / 2;
            char* words[max_words];
            int query_len = 0; 
            bool good_query = true;
            // separating words by whitespace and normalizing them
            normalQuery = normalize_word(query);
            free(query);
            char* word = strtok(normalQuery, " \t");
            // read through each word in the query
            while(word != NULL){
                // check for non-alphabetic characters in each word
                for(int j = 0; j < strlen(word); j++){
                    if (isalpha(word[j]) == 0 && isspace(word[j]) == 0) {
                        // if non-alphabetic, print error and move on to next query
                        printf("ERROR: bad character '%c' in query\n", word[j]);
                        good_query = false;
                        break;
                    }
                }
                // break from loop if current query is bad
                if (!good_query){
                    break;
                }
                // store the word in the array of words
                words[query_len] = word;  
                word = strtok(NULL, " \t");
                query_len++;  
            }
            // move on to next query if the current query is bad
            if (!good_query){
                free(normalQuery);
                good_query = true;
                prompt();
                continue;
            }
            // if the query was not parsed properly, move on to next query
            if (words == NULL){
                prompt();
                continue;
            }            
            // if invalid query structure, move on to next query
            if (!validate_query(words, query_len)){
                prompt();
                continue;
            }
            // print the query
            printf("Query:");
            for (int i = 0; i < query_len; i++){
                printf(" %s", words[i]); 
            }
            printf("\n");
            
            // bag to keep track of all 'andseqeunces' in the query
            bag_t* and_sequences = bag_new();

            // counters to keep track of the 'and' intersection of everything seen so far
            counters_t* and_sofar = counters_new();
            counters_union(and_sofar, index_find(index, words[0]));

            // determine all 'andsequences'
            for (int i = 1; i < query_len; i++){
                // break the current and sequence if an 'or' is seen and start a new 'andsequence'
                if (strcmp(words[i], "or") == 0){
                    bag_insert(and_sequences, and_sofar);
                    and_sofar = counters_new();
                    counters_union(and_sofar, index_find(index, words[i+1]));
                    i++;
                } else {
                    // we are in the middle of an 'andsequence' 
                    if (strcmp(words[i], "and") != 0){
                        // calculate the intersection of the current word and the current 'andsequence'
                        counters_t* curr = index_find(index, words[i]);
                        counters_intersect(and_sofar, curr);
                    }
                }
            }
            bag_insert(and_sequences, and_sofar);

            // calculate the union of all the 'andsequences'
            counters_t* total_union = counters_new();
            counters_t* curr_andseq;
            while ( (curr_andseq = bag_extract(and_sequences)) != NULL){
                counters_union(total_union, curr_andseq);
                counters_delete(curr_andseq);
            }

            // get the number of unique docs in the 'andsequence'
            int numDocs = 0;
            counters_iterate(total_union, &numDocs, count_docs);

            // begin to rank the documents based on their score
            ranking_t** rankarray = calloc(sizeof(ranking_t*), numDocs);
            for(int p = 0; p < numDocs; p++){
                rankarray[p] = NULL;
            }
            // insert each ranking object for each doc into rankarray
            counters_iterate(total_union, rankarray, &counters_insert);
            // sort each ranking object
            rankarray_sort(rankarray, numDocs);
            
            // print number of documents matched
            if (numDocs == 0){
                printf("No documents match\n");
            } else {
                printf("Matches %d documents (ranked):\n", numDocs);
            }

            // print the documents in ranked order
            for (int i = 0; i < numDocs; i++){

                ranking_t* currentRank = rankarray[i];
                char* filename = malloc(strlen(pageDir) + 8);
                if (filename == NULL){
                    break;
                }
                sprintf(filename, "%s/%d", pageDir, currentRank->docID);
                FILE* page = fopen(filename,"r");
                // open file for url
                if (page == NULL){
                    fprintf(stderr, "File %d cannot be opened\n", currentRank->docID);
                    break;
                }
                char* url = freadlinep(page);

                // prints the current document score info
                printf("score\t%d doc\t%d: %s\n", currentRank->score, currentRank->docID, url);
                free(filename);
                free(url);
                fclose(page);
            }
            printf("-----------------------------------------------\n");

            // free dynamic memory
            for (int i = 0; i < numDocs; i++){
                free(rankarray[i]);
            }
            free(rankarray);
            bag_delete(and_sequences, itemdelete);
            counters_delete(total_union);
            free(normalQuery);
            prompt();

        }
        index_delete(index);

    } else {
        fprintf(stderr, "ERROR: usage: ./querier pageDirectory indexFilename\n");
        return 1;
    }
    return 0;
}

/******** prompt *************/
/*
 * We do: 
 *  Print a user prompt if stdin is a terminal
 */
static void prompt(){
  if (isatty(fileno(stdin))) {
    printf("Query? ");
    }
}


/******** validate_query *************/
/*
 * Caller provides:
 *  An pointer to a pointer of strings in the query (a dynamic array of the words in the query)
 *  The number of words in the query
 *
 * We return: 
 *  true if the query doesn't start or end with 'and' or 'or' and the query has no consecutive 'and's or 'or's
 *  false otherwise
 */
bool
validate_query(char** words, const int query_len)
{
    //check if query starts or ends with 'and' or 'or'
    if (strcmp(words[0], "and") == 0){
        fprintf(stderr, "ERROR: 'and' cannot be first item in query\n");
        return false;
    }
    if (strcmp(words[0], "or") == 0){
        fprintf(stderr, "ERROR: 'or' cannot be first\n");
        return false;
    }
    if (strcmp(words[query_len-1], "and") == 0){
        fprintf(stderr, "ERROR: 'and' cannot be last\n");
        return false;
    }
    if (strcmp(words[query_len-1], "or") == 0){
        fprintf(stderr, "ERROR: 'or' cannot be last\n");
        return false;
    }

    //check for consecutive 'and' or 'or's
    for (int i = 1; i < query_len-1; i++){
        if (strcmp(words[i], "and") == 0){
            if (strcmp(words[i+1], "and") == 0){
                fprintf(stderr, "Error: 'and' and 'and' cannot be consecutive\n");
                return false;
            }
            if (strcmp(words[i+1], "or") == 0){
                fprintf(stderr, "Error: 'and' and 'or' cannot be consecutive\n");
                return false;
            }
        }

        if (strcmp(words[i], "or") == 0){
            if (strcmp(words[i+1], "and") == 0){
                fprintf(stderr, "Error: 'or' and 'and' cannot be consecutive\n");
                return false;
            }
            if (strcmp(words[i+1], "or") == 0){
                fprintf(stderr, "Error: 'or' and 'or' cannot be consecutive\n");
                return false;
            }
        }
    }
    return true;
}

/******** counters_intersect *************/
/*
 * Caller provides:
 *  Two pointers to counters structs, the first which will store the intersection
 *
 * We do: 
 *  Store the intersection of the two counters keys and counts in the first counter argument
 */
void 
counters_intersect(counters_t* ct1, counters_t* ct2)
{	
    struct twocts args = {ct1, ct2}; 
    counters_iterate(ct1, &args, intersect_helper);   
}

/******** counters_union *************/
/*
 * Caller provides:
 *  Two pointers to counters structs, the first which will store the union
 *
 * We do: 
 *  Store the union of the two counters keys and counts in the first counter argument
 */
void 
counters_union(counters_t* ct1, counters_t* ct2)
{
    struct twocts args = {ct1, ct2}; 
	counters_iterate(ct2, &args, union_helper);

}

/**************** ranking_new ****************/
/* 
 * Caller provides:
 *  An int key and int count, which reprsent the docID and score for the document, respectively
 *
 * We return:
 *  pointer to the new ranking object, or NULL if error
 * 
 * Caller is responsible for:
 *  later freeing the ranking struct's memory 
 */
ranking_t* 
ranking_new(const int key, const int count)
{
	ranking_t* ranking = malloc(sizeof(ranking_t));
    if (ranking == NULL){
        return NULL;
    } else {
        ranking->docID = key;
        ranking->score = count;
        return ranking;
    }
}

/**************** ranking_new ****************/
/* 
 * Caller provides:
 *  A pointer to pointers of ranking structs (a dynamic array of ranking structs)
 *  An int of the number of ranking structs in the array, or the number of unique documents matching the query
 *
 * We do:
 *   Sort the ranking structs in rankarray in non-increasing order via insertion sort
 * 
 */
static void
rankarray_sort(ranking_t** rankarray, int numDocs)
{
    int i;
    int j;
    ranking_t* currentRank;

    for( i = 1; i < numDocs; i++){
        // keeps copy of current rank
        currentRank = rankarray[i];
        j = i - 1;
        while (j >= 0 && (rankarray[j]->score) < (currentRank->score)){
            rankarray[j+1] = rankarray[j];
            j = j - 1;
        }
        // move one spot over 
        rankarray[j+1] = currentRank;
    }
}



/****************iterator functions****************/

/**************** intersect_helper ****************/
/* 
 * Caller provides:
 *  A pointer to the twocts struct holding two counters struct pointers
 *
 * We do:
 *  Calculate and store the intersection of the two counters in the first counter struct in arg
 * 
 */
void 
intersect_helper(void *arg, const int key, const int count)
{
	struct twocts *two = arg; 
	counters_set(two->result, key, min(count, counters_get(two->another, key)));
}

/**************** union_helper ****************/
/* 
 * Caller provides:
 *  A pointer to the twocts struct holding two counters struct pointers
 *
 * We do:
 *  Calculate and store the union of the two counters in the first counter struct in arg
 * 
 */
void 
union_helper(void *arg, const int key, const int count)
{
	struct twocts *two = arg; 
	counters_set(two->result, key, (count + counters_get(two->result, key)));
}

/**************** count_docs ****************/
/* 
 * Caller provides:
 *  A pointer to the memory of the int to store the number of unique documents in the counters
 *
 * We do:
 *  Calculate and store the number of unique documents in the counters struct in arg's memory
 * 
 */
void 
count_docs(void *arg, const int key, const int count)
{
	int* numDocs = arg;
    if (arg != NULL && key > 0 && count > 0){
        (*numDocs)++;
    }
}

/**************** counters_insert ****************/
/* 
 * Caller provides:
 *  A pointer to pointers of ranking structs that represents a dynamic array of ranking structs
 *
 * We do:
 *  Create a pointer to a new ranking struct where key is the docID and count is the score of the current document
 *  Insert the new ranking struct into the dynamic array of ranking structs
 * 
 */
void 
counters_insert(void *arg, const int key, const int count)
{
    if (count > 0){
        ranking_t** rankarray = (ranking_t**) arg;
        ranking_t* curr = ranking_new(key, count);
        if (curr != NULL){
            int i;
            for(i = 0; rankarray[i] != NULL; i++){}
            rankarray[i] = curr;
        }
    }
}


/****************helper functions****************/

/**************** min ****************/
/* 
 * Caller provides:
 *  Two ints to be compared
 *
 * We return:
 *  The minimum value between the two ints, picking the first argument if the ints have the same value
 * 
 */
static int 
min(const int x, const int y)
{
    if (x > y){
        return y;
    } else {
        return x;
    }
}

/**************** itemdelete ****************/
/* 
 * Caller provides:
 *  A pointer a counters struct
 *
 * We do:
 *  Delete the counters struct from memory
 * 
 */
void 
itemdelete(void* item)
{
    counters_t* curr = (counters_t*) item;
    if (curr != NULL){
        counters_delete(curr);
    }
}
