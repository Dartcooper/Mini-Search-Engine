# CS50 Lab 3
## CS50 Winter 2023

### querier

querier is a directory that contains the contents of the third of three primary parts of the tse lab. Specifically, it has the querier.c which when made and then called with the proper inputs, it will read commands given through standard input, adn it will print the document ID, the associated score of that docID from the given query, and the URL of webpages associated with the docID that are documented in the pageDirectory (that must be a crawler directory) that was passed in the command line. The indexFilename must have an index created by the indexer (ideally the indexFilename should be the index created on the same pageDirectory, but this program will still run based on the information in the indexFilename resulting in bad data).

To test, simply run `make test`.

I used a some of the code from the example file set_iterate2.c specifically for pageor.

No assumptions were made. I changed some of the impimentation specs around the querier method. I changed the way I did some of the query operations and working around the order of things, but it follows the same main idea. I also added a number of smaller helper methods. I would like to note that I understand the side effects of using strtok, and I am careful where I use it. 

I added three simple files for input for testing: goodtestqueries, badtestqueries, mixedtestqueries. I also added some of the sample data for testing from the sample output.

I also implimented full functionality.