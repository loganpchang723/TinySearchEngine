#!/bin/bash

#####################################
## Integration testing for querier ##
#####################################

# variables
tse_dir=/thayerfs/courses/22spring/cosc050/cs50tse/tse-output

# testing with incorrect number of arguments
echo "for" | ./querier 

echo "for" | ./querier $tse_dir/letters-depth-6/

echo "for" | ./querier $tse_dir/letters-depth-6/ $tse_dir/letters-index-10 extra_arg

# testing with invalid parameters
echo "for" | ./querier $tse_dir/letters-depth-10/ $tse_dir/letters-index-6

echo "for" | ./querier $tse_dir/letters-depth-6/ $tse_dir/letters-index-10

# testing with invalid quries
# tesing with non alphabetic characters
echo "for, home" | ./querier $tse_dir/letters-depth-6/ $tse_dir/letters-index-6

echo "for1 home" | ./querier $tse_dir/letters-depth-6/ $tse_dir/letters-index-6

echo "for home!" | ./querier $tse_dir/letters-depth-6/ $tse_dir/letters-index-6

#testing with consecutive 'and' and 'ors'
echo "for and or home" | ./querier $tse_dir/letters-depth-6/ $tse_dir/letters-index-6

echo "for or and home" | ./querier $tse_dir/letters-depth-6/ $tse_dir/letters-index-6

#testing with initial/terminal and and ors
echo "and for home" | ./querier $tse_dir/letters-depth-6/ $tse_dir/letters-index-6

echo "or for home" | ./querier $tse_dir/letters-depth-6/ $tse_dir/letters-index-6

echo "for home and" | ./querier $tse_dir/letters-depth-6/ $tse_dir/letters-index-6

echo "for home or" | ./querier $tse_dir/letters-depth-6/ $tse_dir/letters-index-6

# testing with proper query
echo "search and home or eniac" | ./querier $tse_dir/letters-depth-6/ $tse_dir/letters-index-6

# testing with different spacing
echo "for       home search     first" | ./querier $tse_dir/letters-depth-6/ $tse_dir/letters-index-6

# testing with uppercase letters
echo "fOR AND hoMe" | ./querier $tse_dir/letters-depth-6/ $tse_dir/letters-index-6

# random fuzz testing
./fuzzquery $tse_dir/letters-index-6 10 1211 | ./querier $tse_dir/letters-depth-6/ $tse_dir/letters-index-6

./fuzzquery $tse_dir/toscrape-index-1 10 100 | ./querier $tse_dir/toscrape-depth-1/ $tse_dir/toscrape-index-1

./fuzzquery $tse_dir/toscrape-index-2 10 64 | ./querier $tse_dir/toscrape-depth-2/ $tse_dir/toscapre-index-2

# valgrind test
./fuzzquery $tse_dir/toscrape-index-2 10 23 | valgrind --leak-check=full --show-leak-kinds=all ./querier $tse_dir/toscrape-depth-2/ $tse_dir/toscrape-index-2