#!/bin/bash

#####################################
## Integration testing for crawler ##
#####################################

mkdir ../data
mkdir ../data/letters

#testing with missing arguments
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters

#testing with wrong folder path
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/notrealdir 2

#testing with wrong depth
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters 11

#testing with non-integer maxDepth argument
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters x

#testing with non-existant server
./crawler http://cs50tse.cs.darmuth.edu/tse/letters/index.html ../data/letters 2

#testing with external server
./crawler https://dartmouth.instructure.com/ ../data/letters 2

#testing with valid server, non-existant page
./crawler http://cs50tse.cs.darmuth.edu/tse/letters/fake.html ../data/letters 2

#testing with letters/index.html depth 0, 2, 4, 6
mkdir ../data/letters-0
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters-0 0

mkdir ../data/letters-2
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters-2 2

mkdir ../data/letters-4
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters-4 4

mkdir ../data/letters-6
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters-6 6

#testing with valgrind on letters/index.html depth 5
mkdir ../data/letters-5
valgrind --leak-check=full --show-leak-kinds=all ./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../data/letters-5 5

#testing with toscrape depth 0, 1, 2
mkdir ../data/toscrape-0
./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html ../data/toscrape-0 0

mkdir ../data/toscrape-1
./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html ../data/toscrape-1 1

mkdir ../data/toscrape-2
./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/index.html ../data/toscrape-2 2

# #testing with wikipedia depth 0,1
mkdir ../data/wikipedia-0
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/index.html ../data/wikipedia-0 0

mkdir ../data/wikipedia-1
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/index.html ../data/wikipedia-1 1