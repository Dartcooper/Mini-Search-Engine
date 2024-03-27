/* 
 * word.h - header file for word module
 *
 * takes a word and normalizes it by converting it to all lowercase 
 *
 * Cooper LaPorte Febuary 2023
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


/**************** index_fill ****************/
/* Takes a char* (word) and normalizes it by converting it to all lowercase
 * 
 * Caller provides:
 *   Valid char*
 * Returns:
 *   lowercase version of the word
 * Note:
 *   caller is responsible for freeing the word
 */
char* word_normalize(char* word);