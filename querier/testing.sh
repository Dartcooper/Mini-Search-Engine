#!/bin/bash
### testing script for querier.c


### Making querier.o
make

### Making directories and files for invaliud arguments
# directory with no .crawler file
mkdir ../data/no_crawler
# directory with .crawler file
mkdir ../data/has_crawler
touch ../data/has_crawler/.crawler
# unreadable file
touch ../data/no_reading
chmod -wr ../data/no_reading


# First, a sequence of invocations with erroneous arguments, testing each of the possible mistakes that can be made.
### Calling with no arguments
./querier 

### Calling with one argument
./querier  example_output/data/toscrape-depth-1

### Calling with three arguments
./querier  example_output/data/toscrape-depth-1 example_output/data/toscrape-index-1 45

### Calling with two arguments of wrong types
./querier  234 4534e

### Calling with two arguments with pageDirectory not existing
./querier  ../data/darn_not_here example_output/data/toscrape-index-1

### Calling with two arguments with pageDirectory not having a .crawler file
./querier  ../data/no_crawler example_output/data/toscrape-index-1

### Calling with two arguments with indexFilename not existing
./querier  example_output/data/toscrape-depth-1 ../data/not_here

### Calling with two arguments with indexFilename not being readable
./querier  example_output/data/toscrape-depth-1 ../data/no_reading



# Second, run of valid command-line input and testing invalid queries using fuzztesting.
### Calling with valid page directory and indexFilename and passing a file with a list of invalid queries
### (Including starting and ending with "and" or "or" and having them back to back and having invalid words/characters)
### (All queries should produce output of "Bad query")
./querier  example_output/data/toscrape-depth-1 example_output/data/toscrape-index-1 < badtestqueries



# Third, run of valid command-line inputs and testing valid queries.
### Calling with valid page directory and indexFilename and passing a file with a list of valid queries ending with an empty string
### (All outputs should produce a valid list, no output found, or nothing (when empty string))
### (11 - 15 (just 43) should be identical results and 16 - 18 should be the same as each other (13,42,43,70))
./querier  example_output/data/toscrape-depth-1 example_output/data/toscrape-index-1 < goodtestqueries



# Fourth, a run with valid inputs and some valid and invalid queries running valgrind.
### Run valgrind on a querier with valid page directory and indexFilename and passing a file with a list of invalid queries and valid queries
valgrind --leak-check=full --show-leak-kinds=all ./querier  example_output/data/toscrape-depth-1 example_output/data/toscrape-index-1 < mixedtestqueries