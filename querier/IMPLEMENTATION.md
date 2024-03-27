# CS50 TSE Querier
## Implementation Spec

In this document we reference the Requirements Specification and Design Specification and focus on the implementation-specific decisions.

Here we focus on:

-  Data structures
-  Control flow: pseudo code for overall flow, and for each of the functions
-  Detailed function prototypes and their parameters
-  Error handling and recovery
-  Testing plan

## Data structures 

We use two data structures: a 'counters_t' of docIDs and their scores for a given query, and the 'index' that is a hashtable of words to counters with docIDs and their count.

The counters is empty and fills as docIDs are found matching the given query.
The index is filled at the start based on the indexFilename and is unchagning.

## Control flow

The querier is implemented in one file `querier.c`, with five functions.

### main

The `main` function verifies the arguments by calling `pagedir_hasCrawler` on pageDirectory and creates the index by using code from `indextest` on indexFilename after confirming it is a readable file. Then it calls `querier` assuming all the validation of the commandline arguments passed, then exits zero.
* if any trouble is found, print an error to stderr and exit non-zero.

### querier

Do the real work of the querier by reading queries from the user and returning the associated webpages and their scores.

Pseudocode:

    while stdin is not EOF
        initialize the counters for scores and docIDs called total
        normalize the query and return error if invalid querry
        set a wordA variable to the counters of the normalized first word parsed from the query from the index
        while there is another word to parse from the input line
            make a variable next for the next word
            normalize next
            if next is the word or
                call pageor on total and wordA
                set wordA to the normalized next word parsed from the query's counters from index
            if next is the word and 
                call pageand on wordA and the counters associated with the normalized next word from the index
            else
                call pageand on wordA and the counters associated with next from the index
        call pageor on total and wordA
        call pagerankprint


### pageand

This function implements the *pageand* mentioned in the design.
Given two `counters_t`, find the intersection of the elements and set count to the smaller count of the two.
Pseudocode:

	iterate through the first counters with a itemfunction that:
        if the key is in the second counters
            set the count in the first to the minimum of both counts
        else
            remove the key count pair from the first set


### pageor

This function implements the *pageor* mentioned in the design.
Given two `counters_t`, find the union of the elements and set count to the sum of the two counts.
Pseudocode:

	iterate through the first counters with a itemfunction that:
        if the key is in the second counters
            set the count in the first to the sum of both counts
        else
            add the key count pair to the first set



### pagerankprint

This function implements the *pageranker* mentioned in the design.
Given a `counters_t`, iterate through it finding the docID with the highest score/count and printing the information around that until there are not more docIDs in the counters.
Pseudocode:

	if the counters is empty
        print "No documents match"
    else
        make int maxScoreDocID
    while the counters is not empty
        iterate through counters setting the arg (maxScoreDocID) as the docID with the highest count/score
        print to stdout the score of maxScoreDocID and maxScoreDocID and the URL found by reading the first line of the file named the value of maxScoreDocID in pageDirectory
        remove maxScoreDocID from counters
    


## Other modules

### pagedir

We use the module `pagedir.c` in order to confirm a directory is a crawler directory.

### libcs50

We leverage the modules of libcs50, most notably `hashtable`.
See that directory for module interfaces.

### index

We use the module `index.c` to handle writing an index to a file.

### word

We use the module `word.c` to normalize words

## Function prototypes

### querier

Detailed descriptions of each function's interface is provided as a paragraph comment prior to each function's implementation in `querier.c` and is not repeated here.

```c
int main(const int argc, const char* argv[]);
void querier(const char* pageDirectory, hashtable_t* index);
void pageand(counters_t* ctrsA, counters_t* ctrsB);
void pageor(counters_t* ctrsA, counters_t* ctrsB);
void pagerankprint(counters_t* ctrs, const char* pageDirectory, const char* indexFilename);
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

Integration testing: The querier will be tested by running some queries and testing the standard input arguments. This will be done in a testing script `testing.sh` that invokes the querier several times, with a variety of arguments.
First, a sequence of invocations with erroneous arguments, testing each of the possible mistakes that can be made.
Second, run of valid command-line input and testing invalid queries using fuzztesting.
Third, run of valid command-line inputs and testing valid queries.
Fourth, a run with valid inputs and some valid and invalid queries running valgrind.
Correct behavior will be verified by error messages and studying the output.