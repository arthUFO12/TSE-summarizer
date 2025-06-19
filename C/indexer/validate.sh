#!/bin/bash
# 
# testing.sh
# Created by Arthur Ufongene 05/10/2025
# File that validates the index test

# If files match, indexcmp will run with exit code 0 and output nothing

# letters depth 2
./indexcmp ../data/letters-depth-2/letters.index ../data/letters-depth-2/letters.reindex

# letters depth 10
./indexcmp ../data/letters-depth-10/letters.index ../data/letters-depth-10/letters.reindex

# toscrape depth 1
./indexcmp ../data/toscrape-depth-1/toscrape.index ../data/toscrape-depth-1/toscrape.reindex

# wikipedia depth 1
./indexcmp ../data/wikipedia-depth-1/wikipedia.index ../data/wikipedia-depth-1/wikipedia.reindex

# toscrape depth 0
./indexcmp ../data/toscrape-depth-0/toscrape.index ../data/toscrape-depth-0/toscrape.reindex

# wikipedia depth 0
./indexcmp ../data/wikipedia-depth-0/wikipedia.index ../data/wikipedia-depth-0/wikipedia.reindex
