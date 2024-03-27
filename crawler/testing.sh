#!/bin/bash
### testing script for crawler.c
# This file contains comented out testing that is there in case those test want to be run, but to keep the size of the testing file to a minimum

### Making crawler.o
make

### Making fails directory
mkdir ../data/fail

### Calling with no parameters
./crawler

### Calling with one parameter
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html

### Calling with two parameters
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/fail

### Calling with four parameters
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/fail 0 boop

### Calling with three parameters of incorrect types
./crawler 42 heheha mistake

### Calling with three parameters and an out of bounds maxDepth
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/fail 13

### Calling with three parameters and a maxDepth thats not a number
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/fail hehe

### Calling with three parameters and an invalid url
./crawler http://this.should.not.work ../data/fail 0

### Calling with three parameters and an external url
./crawler https://en.wikipedia.org/wiki/The_Empire_Strikes_Back ../data/fail 0

### Calling with three parameters and a nonexistant pageDirectory
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/failDNE 0

### Calling with three parameters but in the wrong order
./crawler ../data/fail 0 http://cs50tse.cs.dartmouth.edu/tse/letters/index.html

# ### Test valgrind over toscrape at depth 1
# make valgrind

### Test over letters at depth 0
mkdir ../data/letters0
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters0 0

# ### Test over letters at depth 1
# mkdir ../data/letters1
# ./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters1 1

# ### Test over letters at depth 2
# mkdir ../data/letters2
# ./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters2 2

### Test over letters at depth 10
mkdir ../data/letters10
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters10 10

### Test over toScrape at depth 0
mkdir ../data/toScrape0
./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html ../data/toScrape0 0

### Test over toScrape at depth 1
mkdir ../data/toScrape1
./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html ../data/toScrape1 1

# ### Test over toScrape at depth 2
# mkdir ../data/toScrape2
# ./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html ../data/toScrape2 2

# ### Test over toScrape at depth 3
# mkdir ../data/toScrape3
# ./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html ../data/toScrape3 3

### Test over wikipedia at depth 0
mkdir ../data/wikipedia0
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/index.html ../data/wikipedia0 0

### Test over wikipedia at depth 1
mkdir ../data/wikipedia1
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/index.html ../data/wikipedia1 1

# ### Test over wikipedia at depth 2
# mkdir ../data/wikipedia2
# ./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/index.html ../data/wikipedia2 2