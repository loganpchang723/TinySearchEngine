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
#include "file.h"

/*********function prototypes**/
bool pagedir_init(const char* pageDirectory);
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);
bool is_crawler_dir (const char* dir);
webpage_t* pagedir_get (const char* pageDir, const int docID);
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
        if (fp != NULL && page != NULL){
            // get relevant content and print to file      
            fprintf(fp, "%s\n%d\n%s\n", webpage_getURL(page), 
            webpage_getDepth(page), webpage_getHTML(page));    
        }
        fclose(fp);
    }
    free(fname);
}

/**************** is_crawler_dir() ****************/
/* see pagedir.h for description */
bool
is_crawler_dir (const char* dir)
{
    FILE* fp;
    char* pathname = malloc(strlen(dir) + 10);

    // try opening file named "'dir'./crawler"
    sprintf(pathname, "%s/.crawler", dir);
    if ( (fp = fopen(pathname, "r")) != NULL){
        fclose(fp);
        free(pathname);
        return true;
    } else {
        free(pathname);
        return false;
    }
}

/**************** pagedir_get() ****************/
/* see pagedir.h for description */
webpage_t*
pagedir_get(const char* pageDir, const int docID)
{
    if (pageDir !=  NULL && docID > 0){
        // opening file by namne "pagedir/docID"
        char* pathname = malloc(strlen(pageDir) + 8);
        if (pathname == NULL){
            fprintf(stderr, "ERROR: not enough memory to load pathname\n");
            return NULL;
        }
        sprintf(pathname, "%s/%d", pageDir, docID);
        FILE* fp;
        fp = fopen(pathname, "r");
        free(pathname);

        if (fp != NULL){
            // get data from the current file for the webpage struct
            char* html;
            int depth = 0;
            char* url;

            url = freadlinep(fp);
            char* tmp;
            tmp = freadlinep(fp);
            depth = atoi(tmp);
            free(tmp);
            html = freadfilep(fp);

            if (url != NULL && html != NULL && depth >= 0){
                // create the webpage struct
                webpage_t* page = webpage_new(url, depth, html);
                // if page created successfully, return it
                if (page != NULL){
                    fclose(fp);
                    return page;
                } else {
                    fclose(fp);
                    return NULL;
                }
            } else {
                fclose(fp);
                return NULL;
            }
        } else {
            return NULL;
        }
    } else {
        return NULL;
    }
}