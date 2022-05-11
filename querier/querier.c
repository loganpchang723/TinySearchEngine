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
#include "hashtable.h"
#include "file.h"
#include "ctype.h"

/******** local data types *******/
struct twocts {
	counters_t *result;
	counters_t *another;
};

/**************function prototypes*************/
int fileno(FILE *stream);
static void prompt(void); 
// static char** parse_query(char* query, const int max_words);
bool validate_query(char** array, const int query_len);
void counters_intersect(counters_t* ct1, counters_t* ct2); 



void copy_counters(void *arg, const int key, const int count);
void intersect_helper(void *arg, const int key, const int count);
/**************function prototypes*************/

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
        // char** words;

        while ( (query = freadlinep(stdin)) != NULL){
            // parse the query into an array of words
            int max_words = strlen(query) / 2;
            char* words[max_words];
            int query_len = 0; 
            // separating words by whitespace and normalizing them
            char* word = strtok(query, " \t");
            word = normalize_word(word);
            // read through each word in the query
            while(word != NULL){
                // check for non-alphabetic characters in each word
                for(int j = 0; j < strlen(word); j++){
                    if (isalpha(word[j]) == 0 && isspace(word[j]) == 0) {
                        // if non-alphabetic, print error and move on to next query
                        fprintf(stderr, "ERROR: bad character '%c' in query\n", word[j]);
                        free(query);
                        prompt();
                        continue;
                    }
                }
                // store the word in the array of words
                words[query_len] = word;  
                printf("parse_query: just parsed word = %s\n", words[query_len]);
                word = strtok(NULL, " \t");
                word = normalize_word(word);
                query_len++;  
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
            // int query_len;
            printf("Query:");
            for (int i = 0; i < query_len; i++){
                printf(" %s", words[i]); 
            }
            printf("\n");

            /* TESTPOINT 1: CHECK IF QUERY PARSING WORKS */
            
            // bag to keep track of all 'andseqeunces' in the query
            bag_t* and_sequences = bag_new();

            // counters to keep track of the 'and' intersection of everything seen so far
            counters_t* and_sofar = counters_new();
            counters_iterate(index_find(index, words[0]), and_sofar, copy_counters);

            // determine all 'andsequences'
            for (int i = 1; i < query_len; i++){
                // break the current and sequence if an 'or' is seen and start a new 'andsequence'
                if (strcmp(words[i], "or") == 0){
                    bag_insert(and_sequences, and_sofar);
                    and_sofar = counters_new();
                    counters_iterate(index_find(index, words[i+1]), and_sofar, copy_counters);
                } else {
                    // we are in the middle of an 'andsequence' 
                    if (strcomp(words[i], "and") != 0){
                        // calculate the intersection of the current word and the current 'andsequence'
                        counters_t* curr = index_find(index, words[i]);
                        counters_intersect(and_sofar, curr);
                    }
                }
            }
            bag_insert(and_sequences, and_sofar);

            // calculate the union of all the 'andsequences'
            counters_t* total_union = counters_new();
            counters_t* curr;
            while ( (curr = bag_extract(and_sequences) != NULL)){
                counters_union(total_union, curr);
                counters_delete(curr);
            }

            /* TESTPOINT 2: CHECK IF THE AND SEQUENCES WERE CALCULATED PROPERLY */

        }
        

    } else {
        fprintf(stderr, "ERROR: usage: ./querier pageDirectory indexFilename\n");
        return 1;
    }
    return 0;
}

/*
 * TODO: INSERT DOC
 */
static void prompt(){
  if (isatty(fileno(stdin))) {
    printf("Query? ");
    }
}

/*
 * TODO: INSERT DOC
 */
// static char*[] 
// parse_query(char* query, const int max_words)
// {
//     // initialize memory
//     char* words[max_words];
//     int i = 0; 
//     // separating words by whitespace and normalizing them
//     char* word = strtok(query, " ");
//     word = normalize_word(word);
//     // read through each word in the query
//     while(word != NULL){
//         // check for non-alphabetic characters in each word
//         for(int j = 0; j < strlen(word); j++){
//             if (isalpha(word[j]) == 0 && isspace(word[j]) == 0) {
//                 fprintf(stderr, "ERROR: bad character '%c' in query\n", word[j]);
//                 return NULL;
//             }
//         }
//         // store the word in the array of words
//         words[i] = word;  
//         printf("parse_query: just parsed word = %s\n", words[i]);
//         word = strtok(NULL, " ");
//         word = normalize_word(word);
//         i++;  
//     }
//     return words;
// }

/*
 * TODO: INSERT DOC
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

    //check if two and and ors are consecutive
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

/*
 * TODO: INSERT DOC
 */
void 
counters_intersect(counters_t* ct1, counters_t* ct2)
{
	if (ct1 != NULL && ct2 != NULL){
        struct twocts args = {ct1, ct2}; 
	    counters_iterate(ct1, &args, intersect_helper);
    }
}

/*
 * TODO: INSERT DOC
 */
void 
counters_union(counters_t* ct1, counters_t* ct2)
{
	if (ct1 != NULL && ct2 != NULL){
        struct twocts args = {ct1, ct2}; 
	    counters_iterate(ct1, &args, union_helper);
    }
}









/*
 * TODO: INSERT DOC
 */
void
copy_counters(void *arg, const int key, const int count)
{
    if (arg != NULL){
        counters_t* dest = (counters_t*) arg;
        counters_set(dest, key, count);
    }
}

/*
 * TODO: INSERT DOC
 */
void 
intersect_helper(void *arg, const int key, const int count)
{
	struct twocts *two = arg; 
	counters_set(two->result, key, min(count, counters_get(two->another, key)));
}

/*
 * TODO: INSERT DOC
 */
void 
union_helper(void *arg, const int key, const int count)
{
	struct twocts *two = arg; 
	counters_set(two->result, key, (count + counters_get(two->another, key)));
}