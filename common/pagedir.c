/* 
 * pagedir.c - CS50 'pagedir' for TSE
 *
 * see pagedir.h for more information.
 *
 * Logan Chang, 4/29/22
 * CS50, Spring 2022
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "webpage.h"

/*********function prototypes**/
bool pagedir_init(const char* pageDirectory);
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);
/******************/


/**************** pagedir_init() ****************/
/* see pagedir.h for description */
bool 
pagedir_init(const char* pageDirectory)
{
    char* pathname = malloc(strlen(pageDirectory) + 10);

    // create file based on pageDirectory pathname
    sprintf(pathname, "%s/.crawler", pageDirectory);
    if ( (pathname) != NULL){
        FILE* fp = fopen(pathname, "w");
        free(pathname);
        if (fp != NULL){
            fclose(fp);
            return true;
        } else {
            return false;
        }
    } 
    free(pathname);
    return false;
}

/**************** pagedir_save() ****************/
/* see pagedir.h for description */
void
pagedir_save (const webpage_t* page, const char* pageDirectory, const int docID)
{
    // create an arbitray length string buffer for the name of the file to be written to
    char* fname = malloc(strlen(pageDirectory) + 8);
    sprintf(fname, "%s/%d", pageDirectory, docID);
    if (fname != NULL){
        FILE* fp = fopen(fname,"w");
        // free(fname);
        if (fp != NULL && page != NULL){
            // get relevant content and print to file      
            fprintf(fp, "%s\n%d\n%s\n", webpage_getURL(page), 
            webpage_getDepth(page), webpage_getHTML(page));    
        }
        fclose(fp);
    }
    free(fname);
}