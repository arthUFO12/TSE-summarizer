#!/bin/bash
# 
# testing.sh
# Created by Arthur Ufongene 05/19/2025
# File that runs a test for the word module

# Invalid word test
./wordDriver < testingFiles/invalid.txt

# Mix of invalid and valid
./wordDriver < testingFiles/mix.txt