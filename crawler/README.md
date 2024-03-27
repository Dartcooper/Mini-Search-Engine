# CS50 Lab 3
## CS50 Winter 2023

### crawler

crawler is a directory that contains the contents of one of three primary parts of the tse lab. Specifically, it has the crawler.c which when made and then called with the proper inputs, it will scan the webpage given and the URLs on it to the given depth and put all of the pages in files in the given directory. The crawler can be built by running `make` and then run by typing `./crawler A B C` where A is a seedURL that is an 'internal' directory that will be the root webpage, B is a pageDirectory that is the (existing) directory in which to write downloaded webpages, and C is the maxDepth that is an integer in the range [0..10] indicating the maximum crawl depth.

To test, simply run `make test`.

For `make test` and `make valgrind`, you muat run clean before repeating them since the directories are created when run and if there is a duplicate directory, it will not run/overwrite it.

No assumptions were made and I had no important diferences from the specs.