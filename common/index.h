#ifndef __INDEX_H
#define __INDEX_H


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "hashtable.h"
#include "counters.h"
#include <string.h>
#include "file.h"

/****** struct for index type, stores hashtable inside ********/
typedef struct index index_t;
typedef struct counters counters_t;
/*****************************/



/******* index_new *******/
/* initializes an index and an hashtable with size 800. 
 *
 * We return:
 *  Pointer to index struct if initialized succesfully, NULL otherwise
 *
 * Note:
 *  User is responsible for calling index_delete later
 */
index_t* index_new();


/******* index_insert *******/
/* 
 * Caller Provides: 
 *  Pointer to index struct, key for storing, and docID
 *
 * We do:
 *  Insert key into index struct's hashtable and initialize counters (if neccessary)
 *  
 * We return:
 *  true if insert successful, false otherwise
 */
bool index_insert(index_t* index, const char* key, const int docID);

/*********** index_find ************/
/* 
 * Caller provide: 
 *  Pointer to index struct, string of the key to be searched for
 *
 * We return: 
 *  Pointer to counter struct for the given key, NULL if key is not found or an error occurs
 */
counters_t* index_find(index_t* index, const char* key);

/******** index_delete *******/
/* 
 * Caller provides: 
 *  Pointer to index struct to be deleted
 *
 * We do: 
 *  Delete all associated index struct data and free from memory
 *
 */
void index_delete(index_t* index);

/******* index_load *******/
/* 
 * Caller provides: 
 *  File to load index from
 *
 * We return:
 *  Pointer to index struct loaded from the file if successful, NULL otherwise
 */
index_t* index_load(FILE* fp);

/*********** index_save **********/
/* 
 * Caller provides: 
 *  Pointer to index struct and file to write to
 * 
 * We do:
 *  Save to file with format : word docID count [docID count]...
 * 
 * We return:
 *  true if the save was succesful, false otherwise
 */
bool index_save(index_t* index, FILE* fp);

#endif // __INDEX_H