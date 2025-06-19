#!/bin/bash
# 
# testing.sh
# Created by Arthur Ufongene 05/19/2025
# File that runs a test for the querier module

# Invalid arguments test

# No args
./querier

# One arg
./querier ../data/letters-depth-10

# Three args
./querier ../data/letters-depth-10 ../data/letters-depth-10/letters.index ../data

# Non-existent page directory
./querier ../data/beans ../data/letters-depth-10/letters.index

# Not a crawler page directory
./querier ../data ../data/letters-depth-10/letters.index

# Non-existent index file
./querier ../data/letters-depth-10 ../data/letters-depth-11/letters.index


# Run tests with various invalid inputs
./querier ../data/letters-depth-10 ../data/letters-depth-10/letters.index < testingFiles/invalid.txt

# Run tests with fuzzed valid inputs

# On letters
./querier ../data/letters-depth-10 ../data/letters-depth-10/letters.index < testingFiles/letters-10-queries.txt

# On toscrape
./querier ../data/toscrape-depth-1 ../data/toscrape-depth-1/toscrape.index < testingFiles/toscrape-1-queries.txt 

# On wikipedia
./querier ../data/wikipedia-depth-1 ../data/wikipedia-depth-1/wikipedia.index < testingFiles/wikipedia-1-queries.txt


# Valgrind test
valgrind ./querier ../data/wikipedia-depth-1 ../data/wikipedia-depth-1/wikipedia.index < testingFiles/wikipedia-1-queries.txt
