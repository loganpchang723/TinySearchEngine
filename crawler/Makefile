PROG = crawler
OBJS = crawler.o
LLIBS =  ../libcs50/libcs50-given.a ../common/common.a

# add -DNOSLEEP to disable the automatic sleep after web-page fetches
#               (students, please do not use -DNOSLEEP!)
# add -DMEMTEST for memory tracking report in indexer
# (and run `make clean; make` whenever you change this)
FLAGS = # -DMEMTEST  # -DNOSLEEP
CFLAGS = -Wall -pedantic -std=c11 -ggdb $(FLAGS) -I ../libcs50 -I ../common
CC = gcc

crawler: $(OBJS) $(LLIBS)
	$(CC) $(CFLAGS) $(OBJS) $(LLIBS) -o $@

crawler.o: ../libcs50/bag.h ../libcs50/hashtable.h ../libcs50/webpage.h  ../common/pagedir.h crawler.h

.PHONY: test valgrind clean

test: $(PROG)
	bash -v testing.sh >& testing.out

valgrind: $(PROG)
	mkdir ../data/letters-3
	valgrind --leak-check=full --show-leak-kinds=all ./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters-3 3

# clean up after our compilation
clean:
	rm -rf *.dSYM
	rm -f *~ *.o	
	rm -f $(PROG)
