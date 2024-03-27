/* 
 * word.c - CS50 'word' module
 *
 * see word.h for more information.
 *
 * Cooper LaPorte, Febuary 2023
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "mem.h"



/**************** word_normalize ****************/
/* see word.h for description */

char*
word_normalize(char* word){
   char* norm = mem_malloc((strlen(word)+1)*sizeof(char));
   for(int i = 0; word[i]; i++){
    norm[i] = tolower(word[i]);
   }
   norm[strlen(word)]='\0';
   return norm;
}