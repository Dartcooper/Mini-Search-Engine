/* 
 * pagedir.h - header file for pagedirectory
 *
 * Creates files for websites to store information such as URL, depth, and contents
 * Creates .crawler files in given directories
 *
 * Cooper LaPorte January 2023
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "webpage.h"
#include "mem.h"


/**************** functions ****************/

/**************** pagedir_init ****************/
/* Costruct the pathname for a .crawler file in the given directory.
 *
 * Caller provides:
 *   valid directory
 * We return:
 *   true if the .crawler file is created and can be opened for writing
 *   false if that is unsuccessful
 */
bool pagedir_init(const char* pageDirectory);


/**************** pagedir_hasCrawler ****************/
/* Checks if the pageDirectory has a .crawler file
 *
 * Caller provides:
 *   valid directory
 * We return:
 *   true if the .crawler file exists in the given directory
 *   false if it does not
 */

bool pagedir_hasCrawler(const char* pageDirectory);

/**************** pagedir_save ****************/
/* Construct the pathname for the page file in pageDirectory and writes the page
 *
 * Caller provides:
 *   Valid webpage_t, directory, and docID
 * Notes:
 *   File will be created and will have the URL, the depth, and the contents of the webpage
 */
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);
