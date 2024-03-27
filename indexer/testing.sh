#!/bin/bash
### testing script for indexer.c and indextest.c


### Making indexer.o and indextest.o
make

### Making directories and files for invaliud arguments
# directory with no .crawler file
mkdir ../data/no_crawler
# directory with .crawler file
mkdir ../data/has_crawler
touch ../data/has_crawler/.crawler
# read-only directory
mkdir ../data/for_reading
chmod -w ../data/for_reading
# read-only file
touch ../data/file_reading
chmod -w ../data/file_reading

# Test indexer with various invalid arguments
### Calling with no parameters
./indexer

### Calling with one parameter
./indexer ../data/fail

### Calling with three parameters
./indexer ../data/fail ../data/whoops this_wont_work

### Calling with two parameters of incorrect types
./indexer answer_is 42

### Calling with two parameters and an invalid pageDirectory (does not exist)
./indexer  ../data/not_here ../data/whoops

### Calling with two parameters and an invalid pageDirectory (does not have .crawler)
./indexer  ../data/no_crawler ../data/whoops

### Calling with two parameters and an invalid indexFile (path does not exist)
./indexer  ../data/has_crawler ../WHEREAMI/whoops

### Calling with two parameters and an invalid indexFile (in a read-only directory)
./indexer  ../data/has_crawler ../for_reading/whoops

### Calling with two parameters and an invalid indexFile (file exists and is read only)
./indexer  ../data/has_crawler ../data/file_reading

### making crawler and populating some pageDirectories with it
make -C ../crawler
mkdir ../data/letters0
mkdir ../data/letters10
mkdir ../data/toScrape1
mkdir ../data/wikipedia1
../crawler/crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters0 0
../crawler/crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters10 10
../crawler/crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html ../data/toScrape1 1
../crawler/crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/index.html ../data/wikipedia1 1

### Running indexer over letters at depth 0
./indexer  ../data/letters0 ../data/letter0index

### Running indexer over letters at depth 10
./indexer  ../data/letters10 ../data/letter10index

### Running indexer over toscrape at depth 1
./indexer  ../data/toScrape1 ../data/toScrape1index

### Running indexer over wikipedia at depth 1
./indexer  ../data/wikipedia1 ../data/wikipedia1index


### Running indextest on letters at depth 0
./indextest  ../data/letter0index ../data/letter0indexcopy

### Running indextest on letters at depth 10
./indextest  ../data/letter10index ../data/letter10indexcopy

### Running indextest on toscrape at depth 1
./indextest  ../data/toScrape1index ../data/toScrape1indexcopy

### Running indextest on wikipedia at depth 1
./indextest  ../data/wikipedia1index ../data/wikipedia1indexcopy


### Running indexcmp to compare the index from letters at depth 0 and its copy from indextest
./indexcmp  ../data/letter0index ../data/letter0indexcopy

### Running indexcmp to compare the index from letters at depth 10 and its copy from indextest
./indexcmp  ../data/letter10index ../data/letter10indexcopy

### Running indexcmp to compare the index from toscrape at depth 1 and its copy from indextest
./indexcmp  ../data/toScrape1index ../data/toScrape1indexcopy

### Running indexcmp to compare the index from wikipedia at depth 1 and its copy from indextest
./indexcmp  ../data/wikipedia1index ../data/wikipedia1indexcopy


# Run valgrind on both indexer and indextest for letters at depth 6
mkdir ../data/valLetters6
../crawler/crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/valLetters6 6
valgrind --leak-check=full --show-leak-kinds=all ./indexer  ../data/valLetters6 ../data/valLetters6index
valgrind --leak-check=full --show-leak-kinds=all ./indextest  ../data/valLetters6index ../data/valLetters6indexcopy