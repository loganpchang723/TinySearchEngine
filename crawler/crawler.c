/* 
 * crawler.c - CS50 'crawler' for TSE
 *
 * see crawler.h for more information.
 *
 * Logan Chang, 4/29/22
 * CS50, Spring 2022
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "webpage.h"
#include "pagedir.h"
#include "bag.h"
#include "hashtable.h"
#include "crawler.h"
#include <unistd.h>

/********** function prototypes ***********/
int main (const int argc, char* argv[]);
static void crawler (char* seedURL, char* pageDirectory, const int maxDepth);
static void page_scanner (webpage_t* page, bag_t* pageBag, hashtable_t* seen);
/******************************************/

int main (const int argc, char* argv[])
{
    if (argc == 4){

        // parse the arguments
        char* seedURL = argv[1];
        char* pageDir = argv[2];
        int maxDepth;
        char excess;
        if (sscanf(argv[3], "%d%c", &maxDepth, &excess) != 1) {
            fprintf(stderr, "ERROR: maxDepth argument must be an integer\n");
            return 1;
        } 
        // check if arguments fit all criteria, otherwise exit non-zero
        if (!IsInternalURL(seedURL)){
            fprintf(stderr, "ERROR: seedURL is not internal or does not exist\n");
            return 1;
        }
        if (pageDir == NULL){
            fprintf(stderr, "ERROR: page directory is NULL\n");
            return 1;
        }
        if (maxDepth < 0 || maxDepth > 10){
            fprintf(stderr, "ERROR: maxDepth need to be in [0...10]\n");
            return 1;
        }

        // create pageDirectory
        if (pagedir_init(pageDir)){
            crawler(seedURL, pageDir, maxDepth);
        } else {
            fprintf(stderr, "ERROR: page directory not created or writable\n");
            return 1;
        }

    } else {
        fprintf(stderr, "ERROR: usage: crawler seedURL pageDirectory maxDepth\n");
        return 1;
    }
    return 0;
}

/**************** crawler() ****************/
/* see crawler.h for description */
static void 
crawler (char* seedURL, char* pageDirectory, const int maxDepth)
{
    // init data structs
    bag_t* pageBag = bag_new();
    hashtable_t* seen = hashtable_new(100);

    // assert pageBag was allocated properly
    if (pageBag != NULL){
        char* seedCopy = malloc(strlen(seedURL)+1);
        strcpy(seedCopy, seedURL);
        webpage_t* seedPage = webpage_new(seedCopy, 0, NULL);
        // assert seedPage was allocated properly
        if (seedPage == NULL){
            exit(1);
        } else {
            // insert seedPage into the bag of pages to be crawled 
            bag_insert(pageBag, seedPage);
        }
    } else {
        exit(1);
    }

    // assert seen hashtable was allocated properly
    if (seen != NULL){
        // insert seedURL as a seen URL
        hashtable_insert(seen, seedURL, "");
    } else {
        exit(1);
    }

    // extract pages from pageBag and process them
    int currID = 1;
    webpage_t* currPage;
    while ( (currPage = bag_extract(pageBag)) != NULL){
        // try to fetch the current page's HTML
        printf("At depth %d,  fetched URL: %s\n", webpage_getDepth(currPage), webpage_getURL(currPage));
        if (webpage_fetch(currPage)){
            // save the current page to the appropriate file in pageDirectory
            pagedir_save(currPage, pageDirectory, currID);
            // if the current depth is less than maxDepth, scan for URLs on the current page
            if (webpage_getDepth(currPage) < maxDepth){
                page_scanner(currPage, pageBag, seen);             
            }
        } else {
            fprintf(stderr, "ERROR: failed to fetch webpage\n");
            continue;
        }
        // delete the page
        webpage_delete(currPage);
        // increment current docID
        currID++;
    }

    // delete pageBag and seen hashtable
    bag_delete(pageBag, NULL);
    hashtable_delete(seen, NULL);
}

/**************** page_scanner() ****************/
/* see crawler.h for description */
static void
page_scanner (webpage_t* page, bag_t* pageBag, hashtable_t* seen)
{
    int pos = 0;
    char* URL;
    while ( (URL = webpage_getNextURL(page, &pos)) != NULL){
        NormalizeURL(URL);
        if (IsInternalURL(URL)){
            if (hashtable_insert(seen, URL, "")){
                // make a new webpage for that URL
                char* URLcopy = malloc(strlen(URL)+1);
                strcpy(URLcopy, URL);
                webpage_t* newPage = webpage_new(URLcopy, webpage_getDepth(page) + 1, NULL);

                // add the new webpage to the bag of webpages to be crawled
                if (newPage != NULL){
                    bag_insert(pageBag, newPage);
                    printf("At depth %d, added %s to bag\n", webpage_getDepth(page), URL);
                } else {
                    printf("At depth %d, new page at %s not created correctly\n", webpage_getDepth(page), URL);
                }
            } else {
                printf("At depth %d, URL %s seen before\n", webpage_getDepth(page), URL);
            }
        } else {
            printf("At depth %d, URL %s not internal\n", webpage_getDepth(page), URL);
        }
        free(URL);
    }
}