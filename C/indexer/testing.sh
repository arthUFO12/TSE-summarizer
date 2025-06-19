#!/bin/bash
# 
# testing.sh
# Created by Arthur Ufongene 05/10/2025
# File that runs a test for the indexer module

# Invalid arguments test

# No args
./indexer

# One arg
./indexer ../data/letters-depth-2

# Three args
./indexer ../data/letters-depth-2 ../data/letters-depth-2/letters.index ../data

# Non-existent page directory
./indexer ../data/beans ../data/letters-depth-2/letters.index

# Not a crawler page directory
./indexer ../data ../data/letters-depth-2/letters.index

# Non-existent index file
./indexer ../data/letters-depth-2 ../data/letters-depth-11/letters.index

# Index file in read only directory 
./indexer ../data/letters-depth-2 ../read_only/letters.index

# Index file is read only
./indexer ../data/letters-depth-2 ../data/letters-depth-2/files.index


# Run tests on various page directories and validate using indextest
./indexer ../data/letters-depth-2 ../data/letters-depth-2/letters.index
./indextest ../data/letters-depth-2/letters.index ../data/letters-depth-2/letters.reindex

./indexer ../data/letters-depth-10 ../data/letters-depth-10/letters.index
./indextest ../data/letters-depth-10/letters.index ../data/letters-depth-10/letters.reindex

./indexer ../data/toscrape-depth-1 ../data/toscrape-depth-1/toscrape.index
./indextest ../data/toscrape-depth-1/toscrape.index ../data/toscrape-depth-1/toscrape.reindex

./indexer ../data/wikipedia-depth-1 ../data/wikipedia-depth-1/wikipedia.index
./indextest ../data/wikipedia-depth-1/wikipedia.index ../data/wikipedia-depth-1/wikipedia.reindex

./indexer ../data/toscrape-depth-0 ../data/toscrape-depth-0/toscrape.index
./indextest ../data/toscrape-depth-0/toscrape.index ../data/toscrape-depth-0/toscrape.reindex

./indexer ../data/wikipedia-depth-0 ../data/wikipedia-depth-0/wikipedia.index
./indextest ../data/wikipedia-depth-0/wikipedia.index ../data/wikipedia-depth-0/wikipedia.reindex

# Valgrind test
valgrind ./indexer ../data/toscrape-depth-1 ../data/toscrape-depth-1/toscrape.index
valgrind ./indextest ../data/toscrape-depth-1/toscrape.index ../data/toscrape-depth-1/toscrape.reindex
