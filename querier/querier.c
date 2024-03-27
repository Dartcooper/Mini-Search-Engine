/*
 * querier.c - a C script to accept "queries" and print out coresponding information
 * after inputing a proper crawler produced pageDirectory and indexer produced indexFilename
 * it will take queries in standard input after the querier is run until the EOF is called for stdin
 * it will print the document ID, the url associated with it, and 
 * the score that document recived based on the given query
 *
 *
 * Usage: ./querier pageDirectory indexFilename
 * where pageDirectory is an (existing) directory (prodcued by crawler) with a .crawler file in it
 * indexFilename is a readable file that should contain the index of produced by indexer on pageDirectory
 * 
 * Query usage: word (operator) word (operator) word ...
 * where words are the words the user wants to appear in the printed documents
 * and operator can be either ['and', 'or', ' ']
 * using either of ['and', ' '] results in only documents where both words to the left and right appear
 * using 'or' results in documents where either of the left or right words appear
 * 
 * Exit with 0 means succesful
 * Exit with 1 means wrong number of inputs
 * Exit with 2 means wrong type of inputs or inputs out of range
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
#include "pagedir.h"
#include "webpage.h"
#include "index.h"
#include "word.h"
#include "set.h"



/**************** local types ****************/
typedef struct counterspair {
    counters_t* ctrsA;
    counters_t* orCtrs;
} counterspair_t;

typedef struct intpair {
    int docID;
    int score;
} intpair_t;

/**************** counterspair_new ****************/
/* Allocate and initialize a counterspair */
static counterspair_t*
counterspair_new(counters_t* ctrsA, counters_t* orCtrs)
{
  counterspair_t* pair = mem_malloc_assert(sizeof(counterspair_t), "Error allocating memory");
  if (pair == NULL) {
    return NULL;        // error allocating node
  }
  pair->ctrsA = ctrsA;
  pair->orCtrs = orCtrs;
  return pair;
}

/**************** intpair_new ****************/
/* Allocate and initialize a intpair */
static intpair_t*
intpair_new(int docID, int score)
{
  intpair_t* pair = mem_malloc_assert(sizeof(intpair_t), "Error allocating memory");
  if (pair == NULL) {
    return NULL;        // error allocating node
  }
  pair->docID = docID;
  pair->score = score;
  return pair;
}



static void querier(const char* pageDirectory, hashtable_t* index);
static counters_t* pageand(counters_t* ctrsA, counters_t* ctrsB, bool hasWord);
static counters_t* pageor(counters_t* ctrsA, counters_t* ctrsB);
static void pagerankprint(counters_t* ctrs, const char* pageDirectory);
static void counters_and_helper(void* arg, const int key, int count);
static void counters_or_helper(void* arg, const int key, int count);
static void counters_maxscore_helper(void* arg, const int key, const int count);
static char* normalize_line(char* line);
static void counters_delete_helper(void* item);
static void counters_copy_helper(void* arg, const int key, int count);


/* ***************** main ********************** */

int
main(const int argc, const char* argv[])
{
if (argc == 3){
    // two arguments
    if(pagedir_hasCrawler(argv[1])){
      // since only indextest has the method to fill a hashtable from a file, we copy the base code for filling an index here
      FILE* indexFilename = mem_assert(fopen(argv[2], "r"), "*** need to pass readable file for indexFilename");
      hashtable_t* index = mem_assert(hashtable_new(file_numLines(indexFilename)), "Error allocating memory"); // size should be number of words/lines in index
      char* line = NULL;
      while((line = file_readLine(indexFilename)) != NULL){ // as long as there is another line in the file
        char* word = strtok(line, " "); // take word from the line
        char* docID = NULL;
        char* count = NULL;
        counters_t* ctrs = mem_assert(counters_new(), "Error allocating memory");
        while((docID = strtok(NULL, " ")) != NULL){ // continue to take the docID and count pairs and add them to counters
            count = strtok(NULL, " ");
            if(!counters_set(ctrs, atoi(docID), atoi(count))){
              mem_assert(NULL, "Error allocating memory");
            }
        }
        if(!hashtable_insert(index, word, ctrs)){  // add to the hashtable to recreate the one from indexer
          mem_assert(NULL, "Error allocating memory");
        }
        mem_free(line);
      }
      fclose(indexFilename);
      // Index has been created friom the indexFilename
      querier(argv[1], index);
      hashtable_delete(index, counters_delete_helper);
    } else{
      fprintf(stderr,"*** need to pass a valid path to a directory created by crawler\n");
      exit(2);
    }
  } else{
    // too few or many arguments
    fprintf(stderr,"*** need to pass exactly two arguments\n");
    exit(1);
  }
exit(0);
}



/* ****************** querier ********************** */
/*
 * read form standard input queries from the user until the EOF
 * parse the queries and call pageand and pageor until the the whole query has been scanned
 * print out the information on the documents that resulted due to the query
 * 
 */

static void
querier(const char* pageDirectory, hashtable_t* index){
  while(!feof(stdin)){
    char* line;
    printf("\nWhat is your query: ");
    if((line = normalize_line(file_readLine(stdin))) != NULL){ // gets the line and checks if it isnt null
      printf("Query: %s\n", line);    // print the cleaned up query
      counters_t* total = mem_assert(counters_new(), "Error allocating memory"); // create a counters to keep track of all valid documents
      char* word = strtok(line, " "); // take word from the line
      counters_t* wordA = hashtable_find(index, word); // set wordA to the counters of the first word
      bool hasWord = true; // way to keep track if wordA is a counters in the index or a diferent one for memory cleaning purposes
      if(wordA == NULL){ // word is not in the index
        hasWord = false;
        wordA = mem_assert(counters_new(), "Error allocating memory");
      }
      while((word = strtok(NULL, " ")) != NULL){ // continue to take the next word until no more words
        if(wordA == NULL){
          hasWord = true;
          if((wordA = hashtable_find(index, word)) == NULL){ // if the word isnt in the set
            hasWord = false;
            wordA = mem_assert(counters_new(), "Error allocating memory");
          }
        }
        if(strcmp(word, "or") == 0){ // word after wordA is or
          total = pageor(total, wordA); // puts information from wordA into total
          if(!hasWord){
            counters_delete(wordA);
          }
          wordA = NULL;
        } else if(strcmp(word, "and") != 0){ // word after wordA is not an opperator
          if(hashtable_find(index, word) != NULL){ // if the word is in the index
            wordA = pageand(wordA, hashtable_find(index, word), hasWord);
            hasWord = false;
          } else{ // if not in the index, there will be no results in the and chain, clean up
            if(!hasWord){
              counters_delete(wordA);
            }
            wordA = mem_assert(counters_new(), "Error allocating memory");
            hasWord = false;
          }
        }
      }
      total = pageor(total, wordA); // puts information from wordA into total
      if(!hasWord){
        counters_delete(wordA);
      }
      pagerankprint(total, pageDirectory); // print the ranked list of information and delete total
      mem_free(line);
    }
  }
}



/* ****************** pageor ********************** */
/*
 * creates a copy to modify ctrsA such that it includes keys that are in both ctrsA and ctrsB
 * and if the key is in both counters, the count is the sum of the two counts
 * returns the modified counters
 * NOTE:
 *      This method deletes ctrsA! The user is meant replace ctrsA with the returned counters.
 */

static counters_t*
pageor(counters_t* ctrsA, counters_t* ctrsB){
  if(ctrsA != NULL && ctrsB != NULL){
    counters_t* newCtrs = mem_assert(counters_new(), "Error allocating memory");
    counters_iterate(ctrsA, newCtrs, counters_copy_helper); // copy ctrsA into new
    counters_delete(ctrsA);
    counters_iterate(ctrsB, newCtrs, counters_or_helper); // copy ctrsB into new combining count when overlapping
    return newCtrs;
  }
  return NULL;
}



/* ****************** pageand ********************** */
/*
 * creates a new counters to return that is the interesction of ctrsA and ctrsB
 * and the count of the keys that are added is the minimum of the two counts
 * NOTE:
 *      This method deletes ctrsA! The user is meant replace ctrsA with the returned counters.
 */

static counters_t*
pageand(counters_t* ctrsA, counters_t* ctrsB, bool hasWord){
  if(ctrsA != NULL && ctrsB != NULL){
    counters_t* newCtrs = mem_assert(counters_new(), "Error allocating memory");
    counterspair_t* pair = mem_assert(counterspair_new(ctrsA, newCtrs), "Error allocating memory");
    counters_iterate(ctrsB, pair, counters_and_helper);
    if(!hasWord){
      counters_delete(ctrsA);
    }
    mem_free(pair);
    return newCtrs;
  }
  return NULL;
}



/* ****************** pagerankprint ********************** */
/*
 * interates through the given counters to find the docID with the highest score
 * and prints to stdout the score of the docID, the DocID, and the URL of the docID 
 * it "removes" that key from the counters by setting the score to 0 then it repeats until the counters is "empty" 
 * NOTE:
 *      This deletes the given counters.
 */

static void
pagerankprint(counters_t* ctrs, const char* pageDirectory){
  bool empty = true;
  intpair_t* maxPair = mem_assert(intpair_new(0,0), "Error allocating memory"); // docID as 0 so can enter while loop
  while(maxPair->docID >= 0){  // while the docID is an actual docID number
    maxPair->docID = -1;   // make sure if all docIDs have 0 score, loop breaks
    maxPair->score = 0;
    counters_iterate(ctrs, maxPair, counters_maxscore_helper); // find the docID with max score
    if(maxPair->docID >= 0){
      empty = false; // so does not print no documents match
      char* path = mem_malloc_assert(strlen(pageDirectory) + sizeof(maxPair->docID) + 1, "Error allocating memory");
      sprintf(path, "%s/%d", pageDirectory, maxPair->docID);
      FILE* fp = mem_assert(fopen(path, "r"), "Error allocating memory");
      char* url = mem_assert(file_readLine(fp), "Error allocating memory"); // grab url from file
      fclose(fp);
      printf("Score:%d  DocID:%d  URL:%s\n", maxPair->score, maxPair->docID, url); // print info
      mem_free(path);
      mem_free(url);
      if(!counters_set(ctrs, maxPair->docID, 0)){ // make sure this docID is not used again
        mem_assert(NULL, "Error allocating memory");
      }
    }
  }
  mem_free(maxPair);
  if(empty){
    printf("No documents match\n");
  }
  counters_delete(ctrs); // delete the virtually empty counters
}



/* Helper function to use counters_iterate to create a new counters in the pair the intersection of
 * the the other counters in the pair and the one being iterated through
 * and setting the count to the minimum of the two counts if they both have the key
 */
static void 
counters_and_helper(void* arg, const int key, int count)
{
  counterspair_t* pair = arg;  
  // find the same key in ctrsA
  int countA = counters_get(pair->ctrsA, key);
  if (countA == 0) {
    // not found: do nothing
  } else {
    // add key to orCtrs with the minimus of the counts
    if(counters_get(pair->ctrsA, key) <= count){
      if(!counters_set(pair->orCtrs, key, counters_get(pair->ctrsA, key))){
        mem_assert(NULL, "Error allocating memory");
      }
    } else{
      if(!counters_set(pair->orCtrs, key, count)){
        mem_assert(NULL, "Error allocating memory");
      }
    }
  }
}


/* Helper function to use counters_iterate to add the keys from one counters to another
 * and summing their counts if they both have the key
 */
static void 
counters_or_helper(void* arg, const int key, int count)
{
  counters_t* ctrsA = arg;  
  // find the same key in ctrsA
  int countA = counters_get(ctrsA, key);
  if (countA == 0) {
    // not found: insert it
    if(!counters_set(ctrsA, key, count)){
      mem_assert(NULL, "Error allocating memory");
    }
  } else {
    // set count of key = to the sum of the two counts
    countA += count;
    if(!counters_set(ctrsA, key, countA)){
      mem_assert(NULL, "Error allocating memory");
    }
  }
}


/* Helper function to use counters_iterate to copy a counters
 */
static void 
counters_copy_helper(void* arg, const int key, int count)
{
  counters_t* ctrsA = arg;  
  if(!counters_set(ctrsA, key, count)){ // add key/count to the set (for a copy)
      mem_assert(NULL, "Error allocating memory");
  }  
}



/* ****************** counters_maxscore_helper ********************** */
/*
 * Helper function for counters_iterate to find the docID of the max score
 */
static void
counters_maxscore_helper(void* arg, const int key, const int count)
{
  intpair_t* pair = arg;
  if(pair->score < count){
    pair->docID = key;
    pair->score = count;
  }
}


/* ****************** normalize_line ********************** */
/*
 * Helper function to normalize all the words from a line
 * also checks if the line starts or ends with "and" or "or" or has two of those back to back, if bad, return NULL
 * NOTE:
 *      deletes the given string
 */
static char*
normalize_line(char* line)
{
  if (line == NULL){
    return NULL;
  }
  if (strcmp(line, "") == 0){
    mem_free(line);
    return NULL;
  }
  char* normLine = mem_malloc_assert((strlen(line)+1)*sizeof(char), "Error allocating memory");
  strcpy(normLine, "");
  char* word = strtok(line, " ");
  if(word == NULL){
    printf("\n"); // empty query, do nothing
    return NULL;
  }
  for(int i = 0; i<strlen(word); i++){ // loops through each character in a word to make sure it is just letters
    if(isalpha(word[i]) == 0){ // if not a letter, bad input
      mem_free(word);
      mem_free(normLine);
      printf("Bad Query\n");
      return NULL;
    }
  }
  word = word_normalize(word);
  char* last = mem_malloc_assert((strlen(word)+1)*sizeof(char), "Error allocating memory"); // last word in the string
  strcpy(last, "");
  strcpy(last, word);
  if(strcmp(word, "or") == 0 || strcmp(word, "and") == 0){ // if the line starts with and or or, bad query
    mem_free(line);
    mem_free(normLine);
    mem_free(word);
    mem_free(last);
    printf("Bad Query\n");
    return NULL;
  }
  strcat(normLine, word);
  mem_free(word);
  while((word = strtok(NULL, " ")) != NULL){ // continue to take the next word until no more words
    for(int i = 0; i<strlen(word); i++){ // loops through each character in a word to make sure it is just letters
      if(isalpha(word[i]) == 0){ // if not a letter, bad input
        mem_free(word);
        mem_free(normLine);
        printf("Bad Query\n");
        return NULL;
      }
    }
    word = word_normalize(word);
    if((strcmp(word, "or") == 0 || strcmp(word, "and") == 0) && (strcmp(last, "or") == 0 || strcmp(last, "and") == 0)){ // if the line has two and or ors in a row, bad query
      mem_free(normLine);
      mem_free(line);
      mem_free(last);
      mem_free(word);
      printf("Bad Query\n");
      return NULL;
    }
    strcat(normLine, " ");
    strcat(normLine, word);
    last = mem_assert(realloc(last, (strlen(word)+1)*sizeof(char)), "Error allocating memory");
    strcpy(last, "");
    strcpy(last, word);
    mem_free(word);
  }
  mem_free(line);
  if(strcmp(last, "or") == 0 || strcmp(last, "and") == 0){ // if the line ends with and or or, bad query
    mem_free(normLine);
    mem_free(last);
    printf("Bad Query\n");
    return NULL;
  }
  mem_free(last);
  return normLine;
}

/* ****************** counters_delete_helper ********************** */
/*
 * Helper function so I can use counters_delete on the items in the hashtable by casting the void* to counters_t*
 */

static void
counters_delete_helper(void* item){
  counters_t* ctrs = item;
  counters_delete(ctrs);
}