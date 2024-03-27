# CS50 Lab 3
## CS50 Winter 2023

### indexer

indexer is a directory that contains the contents of the second of three primary parts of the tse lab. Specifically, it has the indexer.c which when made and then called with the proper inputs, it will scan the directory given assuming it has the .crawler file and will create or overwrite a file given with every word of length 3 or above appearing in the files with the id of each file the word appears in and the amount of times it appears. The indexer can be built by running `make` and then run by typing `./indexer A B` where A is a pageDirectory that is an (existing) directory that has been populated by the crawler and has a .crawler file, B is an indexFilename that is a file in which to write the recorded words and counts for each word in each file.

To test, simply run `make test`.

The only assumption I made was to add indexcmp to git because it is necessary to run the test and my code does not produce it. For changes to implementation spec, I decided not to make `pagedir_fileToWebpage` that was described in the implimenmtation spec and instead just programed that aspect in the `indexBuild` within indexer.c. For the actual format of the index files produced, I assumed that a single empty line at the end of the file is not an issue given that with my testing, it did not impacted anything or cause problems.