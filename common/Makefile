# Makefile for 'common' module
#
# Logan Chang, CS50 Spring 2022
#
OBJS = pagedir.o word.o index.o
LIB = common.a
# LLIBS = ../libcs50/libcs50-given.a

# add -DNOSLEEP to disable the automatic sleep after web-page fetches
#               (students, please do not use -DNOSLEEP!)
# add -DMEMTEST for memory tracking report in indexer
# (and run `make clean; make` whenever you change this)
FLAGS = # -DMEMTEST  # -DNOSLEEP

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(FLAGS) -I ../libcs50
CC = gcc

# build the library by archiving object files
$(LIB): $(OBJS)
	ar cr $(LIB) $(OBJS)

pagedir.o: ../libcs50/webpage.h ../libcs50/file.h pagedir.h
word.o: word.h
index.o: ../libcs50/hashtable.h ../libcs50/counters.h ../libcs50/file.h index.h


.PHONY: clean


clean:
	rm -rf *.dSYM
	rm -f *~ *.o
	rm -f *.a