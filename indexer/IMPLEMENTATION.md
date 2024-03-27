# CS50 TSE Indexer
## Implementation Spec

In this document we reference the Requirements Specification and Design Specification on Github and focus on the implementation-specific decisions.

Here we focus on the core subset:

-  Data structures
-  Control flow: pseudo code for overall flow, and for each of the functions
-  Detailed function prototypes and their parameters
-  Error handling and recovery
-  Testing plan

## Data structures 

We use four data structures:
'index', a module providing the data structure to represent the in-memory index, and functions to read and write index files
'webpage', a module providing the data structure to represent webpages, and to scan a webpage for words;
'pagedir', a module providing functions to load webpages from files in the pageDirectory;
'word', a module providing a function to normalize a word.

## Control flow

The Indexer is implemented in one file `indexer.c`, with four functions.

### main

The `main` function simply calls `parseArgs` and `indexBuild`, then exits zero.

### parseArgs

Given arguments from the command line, extract them into the function parameters; return only if successful.

* for `pageDirectory`, verifies a valid path to a directory with a .crawler file in it
* for `indexFilename`, verifies path and creates or overwrites the index file and makes sure it can be written in
* if any trouble is found, print an error to stderr and exit non-zero.

### indexBuild

Do the real work of indexing from `pageDirectory` and saving word counts for each page in the `indexFilename`.
Pseudocode:

	initialize the hashtable index
	for each docID in pageDirectory starting from 1
		create a webpage from the lines in the file
		if that was successful,
			call indexPage on index, webpage, and docID
		delete that webpage
    call index_fill on index and indexFilename
	delete the hashtable

### indexPage

Given an `index`, `webpage`, and `docID`, scan the given page for words, ignoring words shorter than 3 letters; add each word to the index, incrementing the counter for that word and docID if it already exists, adding a word, counters pair to the hashtable if the word is not in the hashtable, or adding the docID to the counters for that word if the word already exists but without the docID.
Pseudocode:

	while there is another word in the page
		if that word is more than 2 letters,
            normalize word
            if the word is in the hashtable
                if that word has the docID in the counters
                    increment the counters
                else
                    add a node to the counters with the docID
            else 
                add the word to the hashtable with a new counters
                add a node to the counters with the docID
			
## Other modules

### pagedir

We update the module `pagedir.c` to take a file with a data from a webpage and create a webpage_t from it. We also add a way to check if a directory has been marked with a .crawler file.
We chose to write this as a separate module, in `../common`, to encapsulate all the knowledge about how to initialize and validate a pageDirectory, and how to write and read page files, in one place since some of it will be used by the Crawler, Indexer, and Querier.

Pseudocode for `pagedir_hasCrawler`:

	checks the directory given if it has a .crawler file
	if it does, return true.
	else return false.


Pseudocode for `pagedir_fileToWebpage`:

	checks the directory given if the file is readable
	open that file for reading
	save the first line (the URL) in a variable
	save the second line (the depth) in a variable
	save the third line and onward (the HTML) in a variable
    close the file
    create a webpage with these three variables
    return webpage

### index

We create a re-usable module index.c to handle writing an index to a file.

Pseudocode for `index_fill`:
iterate through hashtable printing with an itemfunction that prints a newline then the key word and iterates through the counters printing the docID and count

### word

We create a re-usable module word.c to normalize words

Pseudocode for `word_normalize`:
create a char* for the normalized word to return
for loop through each character of the given word
	turn each character to lowercase and assign it to that slot of the char*
return the lowercase word

### libcs50

We leverage the modules of libcs50, most notably `counters`, `hashtable`, and `webpage`.
See that directory for module interfaces.
The `webpage` module allows us to represent pages as `webpage_t` objects, to scan a page for words.


## Function prototypes

### indexer

Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's implementation in `indexer.c` and is not repeated here.

```c
int main(const int argc, char* argv[]);
static void parseArgs(const int argc, char* argv[],
                      char** pageDirectory, char** indexFilename);
static void indexBuild(char* pageDirectory, char* indexFilename);
static void indexPage(hashtable_t* index, webpage_t* page, int docID);
```

### pagedir

Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's declaration in `pagedir.h` and is not repeated here.

```c
bool pagedir_init(const char* pageDirectory);
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);
bool pagedir_hasCrawler(const char* pageDirectory);
webpage_t* pagedir_fileToWebpage(const char* file);
```

### index

Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's declaration in `index.h` and is not repeated here.

```c
bool index_fill(hashtable_t* index, const char* file);
```

### word

Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's declaration in `word.h` and is not repeated here.

```c
char* word_normalize(char* word);
```

## Error handling and recovery

All the command-line parameters are rigorously checked before any data structures are allocated or work begins; problems result in a message printed to stderr and a non-zero exit status.

Out-of-memory errors are handled by variants of the `mem_assert` functions, which result in a message printed to stderr and a non-zero exit status.
We anticipate out-of-memory errors to be rare and thus allow the program to crash (cleanly) in this way.

All code uses defensive-programming tactics to catch and exit (using variants of the `mem_assert` functions), e.g., if a function receives bad parameters.

That said, certain errors are caught and handled internally: certain modules will just return false or NULL when unsucceful or passed perameters are bad, but these are not treated as fatal errors.

## Testing plan

Here is an implementation-specific testing plan.

### Integration/system testing

Unit testing: A program indextest will serve as a unit test for the index module; it reads an index file into the internal index data structure, then writes the index out to a new index file.

Integration testing: The indexer will be tested by building an index from a pageDirectory, and then the resulting index will be validated by running it through the indextest to ensure it can be loaded. This will be done in a testing script `testing.sh` that invokes the indexer several times, with a variety of command-line arguments.
First, a sequence of invocations with erroneous arguments, testing each of the possible mistakes that can be made.
Second, a run with valid inputs on a smaller pageDirectory running valgrind.
Third, multiple runs of valid input and running it through indextest.
Correct behavior will be verified by studying the output and comparing files to the ones resulting form the indextest.

