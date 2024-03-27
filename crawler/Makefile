# Makefile for 'crawler' module
#
# Cooper LaPorte January 2023

L = ../libcs50

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I../common -I$L
OBJS = crawler.o
LLIBS = $L/libcs50-given.a ../common/common.a

MAKE = make

# uncomment the following to turn on verbose memory logging
#TESTING=-DMEMTEST

# for memory-leak tests
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

crawler: $(OBJS)
	make -C ../common
	make -C ../libcs50
	$(CC) $(CFLAGS) $^ -o $@ $(LLIBS)

crawler.o: crawler.c

.PHONY: test valgrind clean

test: testing.sh crawler
	./testing.sh

valgrind: crawler
	mkdir ../data/valToScrape
	$(VALGRIND) ./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html ../data/valToScrape 1


clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f *vgcore*
	rm -r -f ../data/*
	rm -f core
	rm -f crawler
	make -C $L clean
	make -C ../common clean