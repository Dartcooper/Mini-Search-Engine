# Makefile for 'common' module
#
# Cooper LaPorte January 2023

L = ../libcs50

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$L
OBJS = pagedir.o word.o index.o
LLIBS = $L/libcs50-given.a

MAKE = make


# uncomment the following to turn on verbose memory logging
#TESTING=-DMEMTEST

common.a: $(OBJS)
	ar cr common.a $^

pagedir.o: pagedir.h
word.o: word.h
index.o: index.h

.PHONY: clean

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f common.a
	rm -f core
	make -C ../libcs50 clean