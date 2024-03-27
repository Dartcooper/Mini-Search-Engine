/* 
 * index.h - header file for index module
 *
 * can take a path to a file and an index hashtable with the node pairs being a word and a counters with each counters key being a docID
 * it will print the hashtable in a specific form in the file
 * 
 *
 * Cooper LaPorte Febuary 2023
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "webpage.h"
#include "mem.h"
#include "hashtable.h"
#include "counters.h"


/**************** index_fill ****************/
/* Fill a file with the information in the hashtable of the form:
 * pairs are a word and a counters with each counters' key being a docID
 * 
 * Caller provides:
 *   Valid pathname to file and hashtable in the form mentioned above
 * Notes:
 *   File will have the contents of the hashtable in index form
 * Returns:
 *   True if the hastable is written to the file
 *   False if the hashtable is null
 */

bool index_fill(hashtable_t* index, const char* file);