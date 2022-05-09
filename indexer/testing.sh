#!/bin/bash

# we assume the data from the crawler is already written to the "../data" directory


#testing with missing and extra arguments
./indexer ../data/letters-0

./indexer 

./indexer ../data/letters-0 ../data/letters-0-index ../data

#testing with wrong directory path
./indexer ../data/fakedir ../data/letters-0-index

#testing with wrong index file to write to
./indexer ../data/letters-0 ../dta/letters-0-index

#testing with a directory not created by the crawler
./indexer ../data/noncrawlerdir ../data/noncrawlerdir-index

#testing with directory as output file
./indexer ../data/letters-0 ../dta/letters-2

#testing with a read-only output file
./indexer ../data/letters-0 ../data/noncrawlerdir/readonly


#testing index test with incorrect number of arguments
./indextest 

./indextest ../data/letters-0-index

./indextest ../data/letters-0-index ../data/letters-2-index ../data/letters-4-index

# testing index test with invalid file arguments
./indextest ../data/letters-0-index ../data/letters-4

./indextest ../data/fake ../data/letters-2-index 



#testing indexer on directory letters-0,2,4
./indexer ../data/letters-0 ../data/letters-0-index

./indextest ../data/letters-0-index ../data/letters-0-index-testing

diff <(sort ../data/letters-0-index) <(sort /thayerfs/courses/22spring/cosc050/cs50tse/tse-output/letters-index-0)

diff <(sort ../data/letters-0-index) <(sort ../data/letters-0-index-testing)


./indexer ../data/letters-2 ../data/letters-2-index

./indextest ../data/letters-2-index ../data/letters-2-index-testing

diff <(sort ../data/letters-2-index) <(sort /thayerfs/courses/22spring/cosc050/cs50tse/tse-output/letters-index-2)

diff <(sort ../data/letters-2-index) <(sort ../data/letters-2-index-testing)


./indexer ../data/letters-4 ../data/letters-4-index

./indextest ../data/letters-4-index ../data/letters-4-index-testing

diff <(sort ../data/letters-4-index) <(sort /thayerfs/courses/22spring/cosc050/cs50tse/tse-output/letters-index-4)

diff <(sort ../data/letters-4-index) <(sort ../data/letters-4-index-testing)


#testing with toscrape depth 0, 1
./indexer ../data/toscrape-0 ../data/toscrape-0-index

./indextest ../data/toscrape-0-index ../data/toscrape-0-index-testing

diff <(sort ../data/toscrape-0-index) <(sort /thayerfs/courses/22spring/cosc050/cs50tse/tse-output/toscrape-index-0)

diff <(sort ../data/toscrape-0-index) <(sort ../data/toscrape-0-index-testing)


./indexer ../data/toscrape-1 ../data/toscrape-1-index

./indextest ../data/toscrape-1-index ../data/toscrape-1-index-testing

diff <(sort ../data/toscrape-1-index) <(sort /thayerfs/courses/22spring/cosc050/cs50tse/tse-output/toscrape-index-1)

diff <(sort ../data/toscrape-1-index) <(sort ../data/toscrape-1-index-testing)


#testing with wikipedia depth 0,1
./indexer ../data/wikipedia-0 ../data/wikipedia-0-index

./indextest ../data/wikipedia-0-index ../data/wikipedia-0-index-testing

diff <(sort ../data/wikipedia-0-index) <(sort /thayerfs/courses/22spring/cosc050/cs50tse/tse-output/wikipedia-index-0)

diff <(sort ../data/wikipedia-0-index) <(sort ../data/wikipedia-0-index-testing)


./indexer ../data/wikipedia-1 ../data/wikipedia-1-index

./indextest ../data/wikipedia-1-index ../data/wikipedia-1-index-testing

diff <(sort ../data/wikipedia-1-index) <(sort /thayerfs/courses/22spring/cosc050/cs50tse/tse-output/wikipedia-index-1)

diff <(sort ../data/wikipedia-1-index) <(sort ../data/wikipedia-1-index-testing)

#testing indexer with valgrind on letters/index.html depth 5
valgrind --leak-check=full --show-leak-kinds=all ./indexer ../data/letters-5 ../data/letters-5-index

#testing indextest with valgrind on letters/index.html depth 5
valgrind --leak-check=full --show-leak-kinds=all ./indextest ../data/letters-5-index ../data/letters-5-index-testing
