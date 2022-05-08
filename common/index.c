#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "hashtable.h"
#include "counters.h"
#include <string.h>
#include "file.h"


/****** struct type ******/
typedef struct index{
    hashtable_t* hashtable;
} index_t;
/*****************************/

/****** function prototypes ********/
index_t* index_new();
bool index_insert(index_t* it, const char* key, const int docID);
counters_t* index_find(index_t* it, const char* key);
void index_delete(index_t* it);
index_t* index_load(FILE* fp);
bool index_save(index_t* it, FILE* fp);
static void counterdelete(void* item);
static void save_item(void* arg, const char* key, void* item);
static void save_counts(void* arg, const int key, const int count);
/*****************************/


/******* index_new() *******/
/* see index.h for description */
index_t* 
index_new()
{
    index_t* index = malloc(sizeof(index_t));
    // try to initialize the index
    if (index != NULL){
        // try to initialize the hashtable
        hashtable_t* ht = hashtable_new(500);
        if (ht != NULL){
            index->hashtable = ht;
            return index;
        } else {
            return NULL;
        }
    } else {
        return NULL;
    }
}


/******* index_new() *******/
/* see index.h for description */
bool 
index_insert(index_t* index, const char* key, const int docID)
{
    // check arguments
    if (index != NULL && key != NULL && docID > 0){
        hashtable_t* ht = index->hashtable;
        if (ht != NULL){
            // check to see if the key is in hashtable already
            void* wordCounter = hashtable_find(ht, key);
            // if the counter for the given word is in the hashtable, increment the counter for that word
            if (wordCounter != NULL){
                if (counters_add(wordCounter, docID) != 0){
                    printf("index_insert: Added word: \"%s\" to the index from docID %d\n", key, docID);
                    return true;
                } else {
                    return false;
                }
            } else {
                // create a new counter for the word
                counters_t* counter = counters_new();
                // increment the counter
                if (counters_add(counter, docID) != 0){
                    // add the counter as the item for the given key in the hashtable
                    return (hashtable_insert(ht, key, counter));
                } else {
                    return false;
                }
            }
        } else {
            return false;
        }
    } else {
        return false;
    }
}

/*********** index_find() ************/
/* see index.h for description */
counters_t* 
index_find(index_t* index, const char* key)
{
    if (index != NULL && key != NULL){
        hashtable_t* ht = index->hashtable;
        if (ht != NULL){
            // find if there is a counter for the key yet (if key exists in hashtable)
            counters_t* counter = hashtable_find(ht, key);
            if (counter != NULL){
                return counter;
            } else {
                return NULL;
            }
        } else {
            return NULL;
        }
    } else {
        return NULL;
    }
}

/******** index_delete() *******/
/* see index.h for description */
void 
index_delete(index_t* index)
{
    if (index != NULL){
        hashtable_t* ht = index->hashtable;
        // delete hashtable struct
        if (ht != NULL){
            hashtable_delete(ht, &counterdelete);
        }
        free(index);
    }
}

/******* index_load() *******/
/* see index.h for description */
index_t* 
index_load(FILE* fp)
{
    if (fp == NULL){
        return NULL;
    }

    // initialize index struct
    index_t* index = index_new();
    if (index == NULL){
        return NULL;
    }

    // initialize variables to be read
    char* ptr;
    int count;
    int docID;
    char* line;

    // get the index struct's hashtable pointer
    hashtable_t* ht = index->hashtable;
    if (ht == NULL){
        return NULL;
    }

    // read all lines in the file
    while ( (line = freadlinep(fp)) != NULL){
        // get the key from this line and copy it
        ptr = strtok(line, " ");
        char* keyCopy = malloc(strlen(ptr)+1);
        strcpy(keyCopy, ptr);

        // int tracker to determine if the read integer is count or docID
        int tracker = 1;

        // while there are integers to read
        while ( (ptr = strtok(NULL, " ")) != NULL){
            // odd trackers are docIDs
            if (tracker % 2 == 1){
                docID = atoi(ptr);
            } else {
                // even trackers are counts
                count = atoi(ptr);
                // put the entry of this word into the index struct
                counters_t* keyCounter = hashtable_find(ht, keyCopy);
                // if key exists, set its count
                if (keyCounter != NULL){
                    counters_set(keyCounter, docID, count);
                } else {
                    // if key doesn't exist, create a new counters struct and insert it into the hashtable
                    counters_t* newCounter = counters_new();
                    counters_set(newCounter, docID, count);
                    hashtable_insert(ht, keyCopy, newCounter);
                }
            }
            tracker++;
            printf("index_load: Added word: \"%s\" to the index with count %d in docID %d\n", keyCopy, count, docID);
        }
        // free dyanmic variables
        free(line);
        free(keyCopy);
    }
    // free(line);
    return index;
}

/*********** index_save() **********/
/* see index.h for description */
bool 
index_save(index_t* index, FILE* fp)
{
    if (index != NULL && fp != NULL){
        hashtable_t* ht = index->hashtable;
        if (ht != NULL){
            hashtable_iterate(ht, fp, &save_item);
            printf("index_save: Successfully saved the index to the index file\n");
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }

}

/************ local functions *****************/

// used in index_save() to save the index to a file
static void 
save_item(void* arg, const char* key, void* item)
{
    if (arg != NULL && key != NULL && item != NULL){
        FILE* fp = arg;
        counters_t* counter = item;
        // print key to the file
        fprintf(fp, "%s ", key);
        counters_iterate(counter, arg, &save_counts);
        fprintf(fp, "\n");
    }


    
}

// used in save_item() to print the words count and docID
static void
save_counts(void* arg, const int key, const int count)
{
    if (arg != NULL){
        // print the count and docID to the file
        FILE* fp =  arg;
        fprintf(fp, "%d %d ", key, count);
    }
}

// used in index_delete() to delete counters struct
static void
counterdelete(void* item)
{
    counters_delete((counters_t*)item);
}
/**************************************/