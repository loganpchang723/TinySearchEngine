#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "index.h"
#include "pagedir.h"
#include "word.h"
#include "counters.h"
#include "hashtable.h"
#include "file.h"
#include "ctype.h"

/**************function prototypes*************/
int fileno(FILE *stream);
static void prompt(void); 
// static char** parse_query(char* query, const int max_words);
bool validate_query(char** array, const int query_len);
// void simple_print(void* arg, const int key, const int count);
// void calculateScore(index_t* it, counters_t* scoreKeeper, char** str, int len);
// void help_accumulateScore(void* arg, const int key, const int count);
// static void itemcount(void *arg, const int key,  const int count);
// static char*** parse_andSequence(char** str, int* count, int len);
// static void andSequenceAccumulate(counters_t* masterScore, char** andSequence, index_t* it);
// static void counters_merge(counters_t* master, counters_t* local);
// static void merge_help(void* arg, const int key, const int count);
// static counters_t* counters_cleanup(counters_t* ctrs, index_t* it, char** str);
// static void simple_set(void* arg, const int key, const int count);
// static void cleanup_help(void* arg, const int key, const int count);
// static void existence_check(void* arg, const int key, const int count);
// static void simple_insert(void* arg, const int key, const int count);
// static void rankBoard_sort(ranking_t** rankBoard, int slot);
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
            /* TODO: FILL IN SOME MORE LOGIC FROM HERE */

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