# Makefile for 'indexer' module
#
# Cooper LaPorte January 2023

L = ../libcs50

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I../common -I$L
OBJS = indexer.o
LLIBS = $L/libcs50-given.a ../common/common.a

MAKE = make

# uncomment the following to turn on verbose memory logging
#TESTING=-DMEMTEST

# for memory-leak tests
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

all: indexer indextest


indexer: $(OBJS)
	make -C ../common
	make -C ../libcs50
	$(CC) $(CFLAGS) $^ -o $@ $(LLIBS)

indexer.o: indexer.c

indextest: indextest.o
	make -C ../common
	make -C ../libcs50
	$(CC) $(CFLAGS) $^ -o $@ $(LLIBS)

indextest.o: indextest.c

.PHONY: test valgrind clean all

test: testing.sh indexer indextest
	bash testing.sh

valgrind: indexer ../crawler/crawler
	mkdir ../data/valLetters6
	../crawler/crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/valLetters6 6
	$(VALGRIND) ./indexer  ../data/valLetters6 ../data/valLetters6index

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f *vgcore*
	rm -f core
	rm -r -f ../data/*
	rm -f indexer
	rm -f indextest
	make -C $L clean
	make -C ../common clean
	make -C ../crawler clean
