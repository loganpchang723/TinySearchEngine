# Makefile for 'querier' 
#
# Logan Chang, CS50 Spring 2022
#
OBJS = querier.o
OBJS1 = fuzzquery.o
LLIBS = ../common/common.a ../libcs50/libcs50-given.a 

# add -DNOSLEEP to disable the automatic sleep after web-page fetches
#               (students, please do not use -DNOSLEEP!)
# add -DMEMTEST for memory tracking report in indexer
# (and run `make clean; make` whenever you change this)
FLAGS = # -DMEMTEST  # -DNOSLEEP

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I../libcs50 -I../common
CC = gcc
MAKE = make

all: querier fuzzquery

querier: $(OBJS) $(LLIBS)
	$(CC) $(CFLAGS) $(OBJS) $(LLIBS) -o querier

fuzzquery: $(OBJS1) $(LLIBS)
	$(CC) $(CFLAGS) $(OBJS1) $(LLIBS) -o fuzzquery

querier.o: ../libcs50/counters.h ../libcs50/hashtable.h ../libcs50/bag.h ../libcs50/file.h  ../common/pagedir.h ../common/word.h ../common/index.h 
fuzzquery.o: ../libcs50/memory.h ../libcs50/file.h

.PHONY: test clean

test: querier fuzzquery
	bash -v testing.sh >& testing.out

valgrind: querier fuzzquery
	./fuzzquery /thayerfs/courses/22spring/cosc050/cs50tse/tse-output/toscrape-index-1 10 214| valgrind --leak-check=full --show-leak-kinds=all  ./querier /thayerfs/courses/22spring/cosc050/cs50tse/tse-output/toscrape-depth-1/ /thayerfs/courses/22spring/cosc050/cs50tse/tse-output/toscrape-index-1

clean:
	rm -rf *.dSYM 
	rm -f *~ *.o
	rm -f querier
	rm -f core
	rm -f fuzzquery