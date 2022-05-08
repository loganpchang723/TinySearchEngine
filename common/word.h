#ifndef __word_H
#define __word_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/******** normalize_word *************/
/*
 * Caller provides:
 *  A string pointer to a word to be normalized
 *
 * We return: 
 *  Normalize the word and return a pointer to it
 */
char* normalize_word(const char* word);

#endif // __word_H