/* 
 * index.c - CS50 'index' module
 *
 * see index.h for more information.
 *
 * Cooper LaPorte, Febuary 2023
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "webpage.h"
#include "mem.h"
#include "hashtable.h"
#include "counters.h"



static void printToFile(void* arg, const char* key, void* item);
static void countersPrint(void* arg, const int key, const int count);

/**************** index_fill ****************/
/* see index.h for description */

bool
index_fill(hashtable_t* index, const char* file){
    if(index != NULL){
        FILE* fp = mem_assert(fopen(file, "w"), "*** file could not be opened"); // should not fail due to other precautions, but just in case
        hashtable_iterate(index, fp, printToFile);
        fclose(fp);
        return true;
    }
    return false;
}


/**************** printToFile ****************/
/* helper function passed into hashtable iterate to print the word then associated docIDs and counts */

static void
printToFile(void* arg, const char* key, void* item){
    FILE* file = arg;
    counters_t* ctrs = item;
    fprintf(file, "%s ", key); // print the word
    counters_iterate(ctrs, file, countersPrint); // iterate through the counters associated with that word
    fprintf(file, "\n"); // print new line
}


/**************** countersPrint ****************/
/* helper function passed into counters iterate to print the associated docIDs and counts */

static void
countersPrint(void* arg, const int key, const int count){
    FILE* file = arg;
    fprintf(file, "%d %d ", key, count); // print the docID and the count assiated with it
}