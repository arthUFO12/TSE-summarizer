# Querier Module
CS50 Tiny Search Engine
Arthur Ufongene — May 2025

# Design spec
The Querier module takes a page directory made by the crawler and an index file made by the indexer and provides a user-facing interface to search the indexed content using logical queries. It retrieves documents from the page directory and displays results ranked by their relevance scores.

## User Interface
The querier's interface with the user is on the command-line and through stdin entering queries. Command line must always have two arguments.

`querier pageDirectory indexFilename`

The program will prompt the user to enter a query, and then the user must enter a query on one line in the syntax below:
```
query       ::= <andsequence> [or <andsequence>]...
andsequence ::= <word> [ [and] <word>]...
```


## Inputs and Outputs
*Input:* The querier will read the given index file, and then reconstruct an index from the data contained. The querier will then take queries from the user to produce scores, accessing the page directory to retrieve URL data.

*Output:* We will output a list of matches in the page directory in the format specified in the requirements

## Expected Modules
The Querier consists of the following primary components:

### Main Program (querier.c)
Handles the overall query process: reading user input, parsing and validating queries, retrieving relevant documents using the index, and printing ranked results using the scoreboard.

### Union Module (union.c)
Provides logic for combining counters (representing sets of documents) using logical conjunction (AND) and disjunction (OR), encapsulating set-based operations for modularity and reuse.

### Word Module (word.c)
Parses and normalizes user input. It also validates the syntax of queries, ensuring logical correctness of expressions.

### Scoreboard Module (scoreboard.c)
Sorts and displays document scores to the user in ranked order, including loading and displaying document content.

## Data structures
### index
Used to map the number occurrences of a word in each document to their respective document. Loaded from an index saved to file.

### union
Encapsulates a counters_t object and provides a clean abstraction for combining sets of document scores using logical operations.
- Disjunction (OR): Combines documents appearing in either set, summing their scores.
- Conjunction (AND): Intersects two sets, using the minimum score for shared document IDs.

### counters
From the libcs50 support library, counters_t is used to map document IDs to integer scores. It forms the backbone of all scoring logic.

### scoreboard
Maintains a sorted array of document scores and associated metadata for presentation. Created from a counters of scores.

### bag
Used internally for iterating over document IDs during conjunction operations.

## Algorithmic flow
The Querier runs as follows:
```
Parses and validates command-line arguments.
Loads the index from a file using index_reconstruct.
While we recieve input from user:
    Prompts the user for input and reads a line.
    calls word_decomposeSequence to parse words from sequence
    Normalizes sequence
    Validates syntax: checks for correct use of and/or.
    If valid, echoes the normalized query.
    Call disjunctOrSequence
    Make a scoreboard from scores
    Print scores
    Clean up
```

DisjunctOrSequence:
```
Create new 'union' object
While there is a next word in the sequence:
    call conjunctAndSequence
    disjunct the result with the union object
    clean up
```

conjunctAndSequence:
```
Create new 'union' object
While there is a next word that isn't an 'or':
    If the word is 'and': skip;
    If this is the first word: 
        get counter from index and disjunct with union
    else: 
        get counter from inex and conjunct with union
```

## TESTING

### 
Querier behavior is tested through a variety of query inputs:

### Invalid inputs:
- Run with no arguments, one, and more than 2 arguments
- Run with invalid page directory
- Run with invalid index file
- Enter empty query
- Enter query with non alphabetic characters
- Enter query with consecutive 'and's or 'or's
- Enter query starting with 'and' or 'or'
- Enter query ending with 'and' or 'or'

### Valid Inputs:
- Queries with one word
- Queries with multiple non-operated words
- Queries with and sequences
- Queries using different combinations 'and's and 'or's
- Large queries with many different operands

### Memory Tests:
Confirm no leaks using valgrind.