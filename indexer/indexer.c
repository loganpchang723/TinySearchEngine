/* 
 * indexer.c - CS50 'indexer' for TSE
 *
 *
 * Logan Chang, 5/9/22
 * CS50, Spring 2022
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "index.h"
#include "word.h"
#include "webpage.h"
#include "pagedir.h"
#include "counters.h"

/****** function prototypes ********/
static void index_build (char* pageDir, FILE* fp);
bool index_page (index_t* index, webpage_t* page, const int docID);
/*****************************/

int main(const int argc, char* argv[])
{
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
        fp = fopen(indexFileName, "w");
        if (fp == NULL){
            fprintf(stderr, "ERROR: index file cannot be opened\n");
            return 1;
        }

        // begin to build the index
        index_build(pageDir, fp);
        fclose(fp);
    } else {
        fprintf(stderr, "ERROR: usage: indexer pageDirectory indexFileName\n");
        return 1;
    }
    return 0;
}
/******* index_build **********/
/* 
 * Caller provides:
 *  The directory to read from and file to wrtie to
 * 
 *
 * We do: 
 *  Make the inverted index of the directory by utilizing index_page (see below) on each page in pageDir and write the results to fp
 * 
 */
static void 
index_build (char* pageDir, FILE* fp)
{
    // build the index and all auxiliary variables
    index_t* index = index_new();
    int docID = 1;
    webpage_t* page;

    // read each page in pageDir
    while ( (page = pagedir_get(pageDir, docID)) != NULL){
        // try to index each page in the directory
        printf("On docID %d, beginning to index page with URL: %s\n", docID, webpage_getURL(page));
        if (index_page(index, page, docID) == false){
            fprintf(stderr, "index_page failed for docID %d in directory %s\n", docID, pageDir);
        }
        docID++;
        webpage_delete(page);
    }
    // save index and cleanup
    index_save(index, fp);
    // printf("index saved\n");
    // webpage_delete(page);
    index_delete(index);
}

/******** index_page *************/
/*
 * Caller provides:
 *  An index struct pointer of the inverted index of the current directory
 *  A webpage struct pointer to the current webpage
 *  The current page's document ID
 *
 * We do: 
 *  Creates the index for the current page 
 *  Returns true if successful, false otherwise
 */
bool 
index_page (index_t* index, webpage_t* page, const int docID)
{
    if (index != NULL && page != NULL && docID > 0){
        // initialize variables
        int pos = 0;
        char* currWord;
        // read each word from page
        while ( (currWord = webpage_getNextWord(page, &pos)) != NULL){
            // ignore words less than 3 characters
                // printf("%s\n", currWord);
            if (strlen(currWord) < 3){
                free(currWord);
            } else {
                // normalize the word
                char* normalWord;
                normalWord = normalize_word(currWord);
                free(currWord);
                // insert the word into the index
                if (normalWord != NULL){
                    if (index_insert(index, normalWord, docID)){
                        free(normalWord);
                    } else {
                        free(normalWord);
                        return false;
                    }
                }
            }
        }
    } 
    return true;
}


