/*
 * indextest.c - a C script to take a file created by indexer.c and converts it to an inverted-index data structure
 * the it writes to the given file in the format of the index
 *
 *
 * Usage: ./indextest oldIndexFilename newIndexFilename
 * where oldIndexFilename is the (existing) file created by indexer in the index format
 * newIndexFilename is a file where we will write the data
 * 
 * Exit with 0 means succesful
 * Exit with 1 means wrong number of inputs
 *
 * Cooper LaPorte, Febuary 2023
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mem.h"
#include "file.h"
#include "counters.h"
#include "hashtable.h"
#include "index.h"



static void counters_delete_helper(void* item);

/* ***************** main ********************** */
/* Read the file that had been created by indexer and recreate the inverted-index hashtable
 * print the inverted-index hashtable to the newfile
 */
int
main(const int argc, char* argv[])
{
if (argc == 3){
    // two arguments
    hashtable_t* index = hashtable_new(200);
    FILE* oldFile = fopen(argv[1], "r");
    char* line = NULL;
    while((line = file_readLine(oldFile)) != NULL){ // as long as there is another line in the file
        char* word = strtok(line, " "); // take word from the line
        char* docID = NULL;
        char* count = NULL;
        counters_t* ctrs = counters_new();
        while((docID = strtok(NULL, " ")) != NULL){ // continue to take the docID and count pairs and add them to counters
            count = strtok(NULL, " ");
            counters_set(ctrs, atoi(docID), atoi(count));
        }
        hashtable_insert(index, word, ctrs); // add to the hashtable to recreate the one from indexer
        mem_free(line);
    }
    fclose(oldFile);
    index_fill(index, argv[2]);
    hashtable_delete(index, counters_delete_helper); // delete hashtable and counters inside
  } else{
    // too few or many arguments
    fprintf(stderr,"*** need to pass exactly two arguments\n");
    exit(1);
  }
exit(0);
}


/* ****************** counters_delete_helper ********************** */
/*
 * Helpper function so I can use counters_delete on the items in the hashtable by casting the void* to counters_t*
 */

static void
counters_delete_helper(void* item){
  counters_t* ctrs = item;
  counters_delete(ctrs);
}