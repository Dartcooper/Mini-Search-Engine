/* 
 * pagedir.c - CS50 'pagedir' module
 *
 * see pagedir.h for more information.
 *
 * Cooper LaPorte, January 2023
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "webpage.h"
#include "mem.h"



/**************** pagedir_init ****************/
/* see pagedir.h for description */

bool
pagedir_init(const char* pageDirectory){
   if(pageDirectory != NULL){
      char* path = mem_malloc_assert(strlen(pageDirectory) + 10, "*** could not allocate memory for path");
      sprintf(path, "%s/.crawler", pageDirectory);
      FILE* fp = mem_assert(fopen(path, "w"), "*** invalid directory, make sure the directory exists");
      mem_free(path);
      if(fp == NULL){
         return false;
      }
      fclose(fp);
      return true;
   }
   return false;
}



/**************** pagedir_hasCrawler ****************/
/* see pagedir.h for description */

bool
pagedir_hasCrawler(const char* pageDirectory){
   if(pageDirectory != NULL){
      char* path = mem_malloc_assert(strlen(pageDirectory) + 10, "*** out of memory");
      sprintf(path, "%s/.crawler", pageDirectory);
      FILE* fp = mem_assert(fopen(path, "r"), "*** invalid directory, make sure the directory exists, can be written, and has a .crawler file");
      mem_free(path);
      if(fp == NULL){
         return false;
      }
      fclose(fp);
      return true;
   }
   return false;
}



/**************** pagedir_save ****************/
/* see pagedir.h for description */

void
pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID){
   if(page != NULL && pageDirectory != NULL && docID >= 0){
      char* path = mem_malloc(strlen(pageDirectory) + sizeof(docID) + 1);
      sprintf(path, "%s/%d", pageDirectory, docID);
      FILE* fp = fopen(path, "w");
      mem_free(path);
      fprintf(fp, "%s\n", webpage_getURL(page));
      fprintf(fp, "%d\n", webpage_getDepth(page));
      fprintf(fp, "%s", webpage_getHTML(page));
      fclose(fp);
   }
}