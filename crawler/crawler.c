/*
 * crawler.c - a C script to "crawl" a website and retrieve various pages and links
 * it puts them all in a directory with each file being a diferent page within the given limits
 *
 *
 * Usage: ./crawler seedURL pageDirectory maxDepth
 * where seedURL is an 'internal' directory, to be used as the initial URL
 * pageDirectory is the (existing) directory in which to write downloaded webpages
 * maxDepth is an integer in range [0..10] indicating the maximum crawl depth
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
#include "set.h"
#include "bag.h"
#include "mem.h"
#include "file.h"
#include "hash.h"
#include "hashtable.h"
#include "pagedir.h"
#include "webpage.h"



static void parseArgs(const int argc, char* argv[],
                      char** seedURL, char** pageDirectory, int* maxDepth);
static void crawl(char* seedURL, char* pageDirectory, const int maxDepth);
static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen);

/* ***************** main ********************** */

int
main(const int argc, char* argv[])
{
if (argc == 4){
    // three arguments
    char* seedURL = NULL;
    char* pageDirectory = NULL;
    int maxDepth = 0;
    parseArgs(argc, argv, &seedURL, &pageDirectory, &maxDepth);
    crawl(seedURL, pageDirectory, maxDepth);
  } else{
    // too few or many arguments
    fprintf(stderr,"*** need to pass exactly three arguments\n");
    exit(1);
  }
exit(0);
}



/* ****************** parseArgs ********************** */
/*
 * Takes the arguments given to crawler.c and checks them
 * normalizes URL and ensure it is internal, 
 * initializes pagedirectory and makes sure it is valid, and makes sure maxDepth is and int and in the range
 */

static void
parseArgs(const int argc, char* argv[],
                      char** seedURL, char** pageDirectory, int* maxDepth){
    char* temp = mem_assert(normalizeURL(argv[1]), "*** need to pass a proper URL");
    *seedURL = temp;
    if(!isInternalURL(*seedURL)){                           // ensures the url is internal
      fprintf(stderr,"*** need to pass an internal URL\n");
      exit(2);
    }
    mem_assert(argv[2], "*** need to pass a proper directory\n");
    *pageDirectory = argv[2];
    if(!pagedir_init(*pageDirectory)){                     // initializes directory and puts the .crawler file in
      fprintf(stderr,"*** page directory failed to initialize, pass valid directory\n");
      exit(2);
    }
    if((atoi(argv[3])) == 0 && strcmp(argv[3], "0") != 0){ // checks if it is a number (&& check if it is 0 because 0 and error return the same atoi)
      fprintf(stderr, "*** need to pass an integer\n");
      exit(2);
    }
    int dep = atoi(argv[3]); // getting the int value of argv[3]
    *maxDepth = dep;
    if(*maxDepth < 0 || *maxDepth > 10){
      fprintf(stderr,"*** need to pass an integer for maxDepth between 0 and 10\n");
      exit(2);
      }
}


/* ****************** crawl ********************** */
/*
 * Scan a webpage at the given the url (assuming it is internal)
 * scan the pages and pages gotten from urls on that page until the given maxdepth
 * put each page in its own file in the pageDirectory given with the url, depth, and content
 * assumes inputs are valid since they had to get through parseArgs
 */

static void
crawl(char* seedURL, char* pageDirectory, const int maxDepth){
  hashtable_t* ht = hashtable_new(200);
  hashtable_insert(ht, seedURL, "");
  bag_t* pages = bag_new();
  webpage_t* toScan = webpage_new(seedURL, 0, NULL);
  bag_insert(pages, toScan);
  int count = 1;
  while((toScan = bag_extract(pages)) != NULL){
    if(webpage_fetch(toScan)){                          // checks if the data/HTML can be found and finds it
      pagedir_save(toScan, pageDirectory, count);       // creates the page with its contents
      count++;                                          // increments count for the IDs
      if(webpage_getDepth(toScan) < maxDepth){
        pageScan(toScan, pages, ht);                    // checks for more URLs if not already max depth
      }
    }
    webpage_delete(toScan);
  }
hashtable_delete(ht, NULL);                             // clean up since done with these
bag_delete(pages, webpage_delete);
}


/* ****************** pageScan ********************** */
/*
 * Scan all of the URLs on the page and add the internal ones to the hashtable of pagesSeen
 * if that URL/webpage was not in pagesSeen, adds it to the bag of pagesToCrawl
 */

static void
pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen){
  int pos = 0;
  char* url;
  while ((url = webpage_getNextURL(page, &pos)) != NULL) {
    // as long as there is an unvisited url on the page
    if(isInternalURL(url)){
      // make sure url is internal
      if(hashtable_insert(pagesSeen, url, "")){
        bag_insert(pagesToCrawl, webpage_new(url, (webpage_getDepth(page)+1), NULL));
        // only free url if not inserted into hashtable, otherwise hashtable will free it
      } else{
        mem_free(url);
      }
    } else {
      mem_free(url);
    }
  }
}