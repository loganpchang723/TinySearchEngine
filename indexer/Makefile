# Makefile for 'indexer' 
#
# Logan Chang, CS50 Spring 2022
#

OBJS = indexer.o
OBJS1 = indextest.o
LLIBS = ../common/common.a ../libcs50/libcs50-given.a 

# add -DNOSLEEP to disable the automatic sleep after web-page fetches
#               (students, please do not use -DNOSLEEP!)
# add -DMEMTEST for memory tracking report in indexer
# (and run `make clean; make` whenever you change this)
FLAGS = # -DMEMTEST  # -DNOSLEEP

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I../libcs50 -I../common
CC = gcc
MAKE = make
all: indexer indextest
indexer: $(OBJS) $(LLIBS)
	$(CC) $(CFLAGS) $(OBJS) $(LLIBS) -o indexer

indextest: $(OBJS1) $(LLIBS)
	$(CC) $(CFLAGS) $(OBJS1) $(LLIBS) -o indextest

indexer.o: ../libcs50/counters.h ../libcs50/webpage.h  ../common/pagedir.h ../common/word.h ../common/index.h 
indextest.o: ../libcs50/file.h

.PHONY: test valgrind clean

test: testing.sh
	bash -v testing.sh >& testing.out

valgrind: indexer indextest
	valgrind --leak-check=full --show-leak-kinds=all ./indexer ../data/letters-3 ../data/letters-3-index
	valgrind --leak-check=full --show-leak-kinds=all ./indextest ../data/letters-3-index ../data/letters-3-index-testing

clean:
	rm -rf *.dSYM 
	rm -f *~ *.o
	rm -f indexer
	rm -f core
	rm -f indextest