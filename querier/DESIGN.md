# CS50 TSE Querier
## Design Spec

In this document we reference the Requirements Specification and focus on the implementation-independent design decisions.

Here we focus on:

- User interface
- Inputs and outputs
- Functional decomposition into modules
- Pseudo code (plain English-like language) for logic/algorithmic flow
- Major data structures
- Testing plan

## User interface

As described in the Requirements Spec, the querier's interface with the user to run the program is on the command-line; it must always have two arguments.

```bash
$ querier pageDirectory indexFilename
```

For example, to query one of the indexes (lettersindex) created by indexer over the crawler created directory (letters6), use this command line:

``` bash
$ ./querier ../data/letters6 ../data/lettersindex
```


Once the querier is run on a valid pageDirectory and indexFilename, it expects "queries" on the commanline until the user calls for EOF for standard input. The queries are of the form:

``` bash
$ wordA and wordB or wordC wordD
```

or any variation of this with zero or more words. Typing `and` means the returned website must have both of those words around the `and`. Typing `or` means the returned website must have at least one of those words around the `or`. No word between two words is the same as typing `and`. `and` takes precident over `or`. In the above example, websites with wordA and wordB will be returned as well as websites with wordC and wordD. There must be a word to the imediate left and right of an `and` and an `or` or else there will be a syntax error. See requironments spec for more examples.

## Inputs and outputs

*Input:* there are command-line parameters described above. There is also standard input that is required to run the querier described above.

*Output:* Per the requirements spec, Querier will return the resulting websites that satisfy the given query in order of score (based on how many occurances of the queried words appear in the website).

## Functional decomposition into modules

We anticipate the following modules or functions:

 1. *main*, which parses arguments and initializes other modules
 2. *querier*, which waits and reads standard input, parses that input and gets pages with those words
 3. *pageand*, which finds the intersection of two sets of words
 4. *pageor*, which finds the union of two sets of words
 5. *pageranker*, which takes the returned pages form a query and scores and orders them


And some helper modules that provide data structures:

  1. *index*, a module providing the data structure to represent the in-memory index, and functions to read and write index files;
  2. *hashtable* of word to counters for the index
  3. *pagedir*, a module providing functions to load webpages from files in the pageDirectory;
  4. *word*, a module providing a function to normalize a word.


## Pseudo code for logic/algorithmic flow

The querier will run as follows:

    parse the command line, validate parameters, initialize other modules
    load index from indexFilename
    read from stdin while not EOF
        parse query word by word normalizing each one
        while there is another word in the query
            if syntax has an error
                break loop and display message about invalid query, return to waiting for query from stdin
            if between two words there is a space or an and
                group those words together by calling pageand on them
            if there is an or between two words
                check what is between the next two words
                if an and or space
                    combine those with pageand
                else
                    add the result to the left of the or to the list of webpages and their score
        call pageranker
        print the ranked order of pages



The querier completes and exits when the user calls EOF on standard input


## Major data structures

Helper modules provide all the data structures we need:

- *counters* of scores and docIDs

## Testing plan


A sampling of tests that should be run:

1. Test the program with various forms of incorrect command-line arguments to ensure that its command-line parsing, and validation of those parameters, works correctly.

2. Query a simple, closed set of cross-linked web pages like [letters](http://cs50tse.cs.dartmouth.edu/tse/letters/), at max depth of 10.
Verify that the query is returning correct information.

3. Repeat for a larger set like wikipedia at depth 2 or even 3 (this may take the crawler a fair bit of time to do).
