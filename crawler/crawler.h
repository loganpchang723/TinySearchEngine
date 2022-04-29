#ifndef __crawler_H
#define __crawler_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "webpage.h"
#include "pagedir.h"
#include "bag.h"
#include "hashtable.h"
#include <unistd.h>


/******* crawler **********/
/* 
 * Caller provides:
 *  The seedURL, pageDirectory, and maxDepth arguments passed to the crawler
 *  A bag struct pointer to the pages to explore
 *  A hashtable struct pointer to the pages already seen
 *
 * We do: 
 *  Pass the explored page's URL to page_fetcher
 * 
 */
static void crawler (char* seedURL, char* pageDirectory, const int maxDepth);

/******** page_scanner *************/
/*
 * Caller provides:
 *  A webpage struct pointer to the webpage to be parsed
 *  A bag struct pointer to the pages to explore
 *  A hashtable struct pointer to the pages already seen
 *
 * We do: 
 *  Extract URLs from the webpage and processes each one
 */
static void page_scanner (webpage_t* page, bag_t* pages, hashtable_t* seen);


#endif // __crawler_H