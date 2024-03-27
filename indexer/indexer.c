/*
 * indexer.c - a C script to "index" a directory of files produced by crawler
 * it writes the found words to the given file with each file the word occured in and the amount of times it occured
 *
 *
 * Usage: ./indexer pageDirectory indexFilename
 * where pageDirectory is the (existing) directory with a .crawler file in it which to read files/webpages
 * indexFilename is a file that can be existing or not to write the data about the words and files
 * 
 * Exit with 0 means succesful
 * Exit with 1 means wrong number of inputs
 * Exit with 2 means wrong type of inputs or inputs out of range
 *
 * Cooper LaPorte, January 2023
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mem.h"
#include "file.h"
#include "counters.h"
#include "hashtable.h"
#include "pagedir.h"
#include "webpage.h"
#include "index.h"
#include "word.h"



static void parseArgs(const int argc, char* argv[],
                      char** pageDirectory, char** indexFilename);
static void indexBuild(char* pageDirectory, char* indexFilename);
static void indexPage(hashtable_t* index, webpage_t* page, int docID);
static void counters_delete_helper(void* item);

/* ***************** main ********************** */

int
main(const int argc, char* argv[])
{
if (argc == 3){
    // two arguments
    char* indexFilename = NULL;
    char* pageDirectory = NULL;
    parseArgs(argc, argv, &pageDirectory, &indexFilename);
    indexBuild(pageDirectory, indexFilename);
  } else{
    // too few or many arguments
    fprintf(stderr,"*** need to pass exactly two arguments\n");
    exit(1);
  }
exit(0);
}



/* ****************** parseArgs ********************** */
/*
 * Takes the arguments given to indexer.c and checks them
 * checks the pagedirectory and makes sure it is valid (has a .crawler file)
 * checks indexFilename and creates or truncates a file for it
 */

static void
parseArgs(const int argc, char* argv[],
                      char** pageDirectory, char** indexFilename){
    *pageDirectory = mem_assert(argv[1], "*** need to pass a proper directory");
    if(!pagedir_hasCrawler(*pageDirectory)){                // Ensures pageDirectory exists with the .crawler file
      fprintf(stderr,"*** page directory failed to initialize, pass valid directory\n");
      exit(2);
    }
    *indexFilename = mem_assert(argv[2], "*** need to pass a proper file pathname");
    FILE* fp = mem_assert(fopen(*indexFilename, "w"), "*** need to pass a proper file pathname (path exists, directory and file are not read only)");
    fclose(fp);      // creates file for indexFilename after ensuring it is a path, closes it since no writing now
}


/* ****************** indexBuild ********************** */
/*
 * Scan each file in the directory given from 1 incrementing by 1 until we run out
 * scan the files/pages and create a webpage_t for each, sending it to indexPage
 * assumes inputs are valid since they had to get through parseArgs
 */

static void
indexBuild(char* pageDirectory, char* indexFilename){

  hashtable_t* index = hashtable_new(200); // 200 is arbitrary, hopefully large enough, but no specific reason
  int docID = 1;
  FILE* read = NULL;
  char* path = mem_malloc(strlen(pageDirectory) + sizeof(docID) + 1);
  sprintf(path, "%s/%d", pageDirectory, docID); // create the path for the first file
  while((read = fopen(path, "r")) != NULL){ // while there is another file named one number higher than the last
    mem_free(path);
    char* URL = file_readLine(read);
    char* depStr = file_readLine(read);
    int depth = atoi(depStr);
    mem_free(depStr);
    char* HTML = file_readFile(read);
    fclose(read);
    webpage_t* page = webpage_new(URL, depth, HTML);
    if (page != NULL){
      indexPage(index, page, docID);
    }
    webpage_delete(page);
    docID++;
    path = mem_malloc(strlen(pageDirectory) + sizeof(docID) + 1);
    sprintf(path, "%s/%d", pageDirectory, docID); // create the path for the first file
  }
  mem_free(path);
  index_fill(index, indexFilename); // actually writting the information gathered to file
  hashtable_delete(index, counters_delete_helper); // delete hashtable and counters inside
}


/* ****************** indexPage ********************** */
/*
 * Scan all of the words on the page and add the longer than 2 letter ones to the hashtable
 * if a word hasn't been seen, add the word as the key and a counters, then add the docID to the counters
 * if seen, but the docID hasn't been added, add docID to counters
 * if the word and docID already exist in the hashtable, increment the counter
 */

static void
indexPage(hashtable_t* index, webpage_t* page, int docID){
  int pos = 0;
  char* word;
  char* wordNorm;
  while ((word = webpage_getNextWord(page, &pos)) != NULL) {
    // as long as there is an unvisited word on the page
    if(strlen(word) > 2){    // as long as the word is longer than 2 letters
      wordNorm = word_normalize(word);      // normalizes the word
      if(hashtable_find(index, wordNorm) == NULL){ // if the word is not in the hashtable
        hashtable_insert(index, wordNorm, counters_new());           // insert the word
      }
        counters_add(hashtable_find(index, wordNorm), docID); // increment or add new node to counters
        mem_free(wordNorm);
    }
    mem_free(word);
  }
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