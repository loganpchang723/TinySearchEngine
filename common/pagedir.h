#ifndef __pagedir_H
#define __pagedir_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "webpage.h"
#include "file.h"

/******* pagedir_init **********/
/* 
 * Caller provides:
 *  Pathname to pageDirectory
 *
 * We return: 
 *  true if the pathname provded as pageDirectory is a writable directory 
 *  false otherwise
 * 
 */
bool pagedir_init (const char* pageDirectory);

/******* pagedir_save **********/
/* 
 * Caller provides: 
 *  Valid webpage pointer, pathname to pageDirectory, valid documentID (greater than 0)
 * 
 * We do:
 *  Print the URL, depth, and HTML content of the page provided into a file of form 'pageDirectory/docID'
 */
void pagedir_save (const webpage_t* page, const char* pageDirectory, const int docID);

/******* is_crawler_dir **********/
/* 
 * Caller provides: 
 *  A directory name
 * 
 * We do:
 *  Return true if dir is a directory created by the crawler, otherwise return false
 */
bool is_crawler_dir (const char* dir);

/******* pagedir_get **********/
/* 
 * Caller provides: 
 *  A directory name and document ID
 * 
 * We do:
 *  Returns a pointer to the webpage struct of the page named "dir/docID", NULL otherwise
 */
webpage_t* pagedir_get (const char* pageDir, const int docID);


#endif // __pagedir_H