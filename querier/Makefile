# Makefile for 'querier' module
#
# Cooper LaPorte Febuary 2023

L = ../libcs50

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I../common -I$L
OBJS = querier.o
LLIBS = ../common/common.a $L/libcs50-given.a

MAKE = make

# uncomment the following to turn on verbose memory logging
#TESTING=-DMEMTEST

# for memory-leak tests
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

querier: $(OBJS)
	make -C ../common
	make -C ../libcs50
	$(CC) $(CFLAGS) $^ -o $@ $(LLIBS)

querier.o: querier.c

.PHONY: test valgrind clean

test: testing.sh querier
	bash testing.sh

valgrind: querier
	$(VALGRIND) ./querier  example_output/data/toscrape-depth-1 example_output/data/toscrape-index-1 < mixedtestqueries


clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f *vgcore*
	rm -r -f ../data/*
	rm -f core
	rm -f querier
	make -C $L clean
	make -C ../common clean
