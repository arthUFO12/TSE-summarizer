#!/bin/bash
# 
# testing.sh
# Created by Arthur Ufongene 05/04/2025
# File that runs a test for the crawler module

# Logging is decreased for these runs. Only fetch messages will be printed

# Test incorrect number of command line args
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/

# Test depth that is out of range or incorrect format
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ ../data/letters-depth-2 11
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ ../data/letters-depth-2 9.5
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ ../data/letters-depth-2 3a
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ ../data/letters-depth-2 a3

# Test URL that is not internal
./crawler http://www.google.com/ ../data/letters-depth-2 2

# Test directory that does not exist
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ ../data/letters-death-2 2

# Valgrind test
valgrind ./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/ ../data/toscrape-depth-1 1

# Test over all sites
./crawler http://cs50tse.cs.dartmouth.edu/tse/toscrape/ ../data/toscrape-depth-0 0
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ ../data/letters-depth-0 0
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ ../data/letters-depth-2 2
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ ../data/letters-depth-10 10
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/ ../data/wikipedia-depth-0 0
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/ ../data/wikipedia-depth-1 1




