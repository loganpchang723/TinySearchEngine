/* 
 * word.c - CS50 'word' for TSE
 *
 * see word.h for more information.
 *
 * Logan Chang, 5/9/22
 * CS50, Spring 2022
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/****** function prototypes ********/
char* normalize_word(const char* word);
/*****************************/

/******** normalize_word() *************/
/* see word.h for description */
char* 
normalize_word(const char* word)
{
    if (word == NULL){
        return NULL;
    }
    //malloc space for word copy and copy it
    char* normalized = malloc(strlen(word)+1);
    strcpy(normalized, word);
    int i = 0;

    // go through the word by each character and normalize each character
    while (normalized[i] != '\0'){
        normalized[i] = tolower(normalized[i]);
        i++;
    }
    return normalized;
}