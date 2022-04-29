# Makefile for 'common' module
#
# Logan Chang
#
OBJS = pagedir.o
LIB = common.a
LLIBS = ../libcs50/libcs50-given.a

# add -DNOSLEEP to disable the automatic sleep after web-page fetches
#               (students, please do not use -DNOSLEEP!)
# add -DMEMTEST for memory tracking report in indexer
# (and run `make clean; make` whenever you change this)
FLAGS = # -DMEMTEST  # -DNOSLEEP

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(FLAGS) -I ../libcs50
CC = gcc

# build the library by archiving object files
$(LIB): $(OBJS)
	ar cr $(LIB) $(LLIBS) $(OBJS)

pagedir.o: ../libcs50/webpage.h


.PHONY: clean


clean:
	rm -rf *.dSYM
	rm -f *~ *.o
	rm -f *.a