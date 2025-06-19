# CS50 TSE Querier
# Implementation Spec
Arthur Ufongene — May 2025

## Data Structures
The Querier uses the following data structures:

### `union_t`
Encapsulates a counters_t object and provides an abstraction for computing logical conjunctions (AND) and disjunctions (OR) of document scores. Internally, it relies on counters_t from libcs50.

### `counters_t`
Maps document IDs to their relevance scores (occurrence counts). Serves as the primary data structure for tracking and combining document scores.

### `index_t`
Maps words to document IDs and the number of occurrences of the word in that document

### `scoreboard_t`
Holds a sorted array of document-score entries for display to the user. The entries are sorted in descending order by score using qsort.

### `bag_t`
Used internally in union_conjunction() to iterate over a copy of keys in a counters set (simulating iteration over a hashset).

## Control Flow
The querier is contained in one file, `querier.c` with 5 functions
### `main`
Initializes arguments and then initiates the query prompt cycle
```
Call parseArgs
Reconstruct index from given file
While we can read a line from stdin:
    Decompose the query
    Normalize and check syntax
    Get scores from query
    Make scoreboard of scores
    Print scores
    Clean up
```

### `prompt`
Prompts the user. Prints "Query? " to stdout if keyboard input.

### `parseArgs`
```
Ensure there are only 2 arguments
Check that the page directory is a crawler directory
Check that the index file can be read from
```

### `disjunctOrSequence`
Handles the top-level query logic. Combines AND-chained subqueries into a total result using OR logic. The location in the sequence is kept track of by an integer that we pass a pointer of to conjunctAndSequence.
```
create union
While there is a next word:
    skip "or"
    Call conjunct and sequence
    Disjunct the result with the union
    Clean up result
return the counter of the union
```

### `conjunctAndSequence`
Handles AND-chained word sequences. It computes the intersection of document scores using the minimum value for each shared docID.
```
create union
While there is a next word and it isn't 'or':
    if 'and': skip;
    if first word: disjunction with counter;
    else: conjunction with counter;
return the counter of the union
```

## Other modules

### `index.c`
Used to load an index from a saved index file. Fully implemented in last lab. Added one new function in `index_get`.
*index_reconstruct*: Reconstructs in memory index from saved index file
*index_get*: Returns a counter for a word in the index.

### `union.c`
This is a wrapper class for the counters object. It merely consists of a pointer to a counter.
*union_new*: Creates a new union object
*union_delete*: Deletes entire union structure including the counter
*union_pointerDelete*: Deletes only the union, not its counter
*union_getCounter*: Returns the union's counter
*union_disjunction*:
```
Iterate over all counter entries:
    Add entry to corresponding entry in union
```
*union_conjunction*:
```
Make new counters object
Iterate over all counter entries:
    Add entry to a bag object
While there are keys in the bag:
    If the key is in the union:
        If the value of the key in the union is less than the value in the counter:
            Add union value to the new counters object
        else:
            Add counter value to the new counters object
Delete previous counter from union
Replace it with new counters object
```

### `word.c`
Module provides functions to create and process sequences of words
*word_decomposeSequence*:
```
allocate a sequence of strings
While we are not at the end of the given string:
    If we see a non-alphabetic or non-space character: return nothing;
    If we aren't in a word and we see an alphabetic character:
        Assign the beginning of the next word in the sequence to this character
    If we are in a word and we see a space:
        Insert a null terminator in this position
Set the rest of the array of strings to NULL
```
*word_normalizeSequence*: Normalizes each word in a sequence and changes the original string to be the normalized version
*word_checkSyntax*:
```
For every word in the sequence:
    If the word is an 'and' or 'or':
        If we just saw an 'and' or 'or': return false;
        else if: this is the first word in the sequence: return false;
        else: set a marker denoting we just saw an 'and' or 'or'
    else:
        set a marker denoting we didn't just see an 'and' or 'or'
if we just saw an 'and' or 'or': return false;
return true
``` 
*word_printSequence*: Prints every word in a sequence to stdout

### `scoreboard.c`
*scoreboard_new*: Creates a new scoreboard object and sorts the scores within it
*scoreboard_print*: Prints the number of entries in the scoreboard followed by the actual scores


## Function Prototypes

#### `index.c`
```c
index_t* index_reconstruct(char* oldFilename);
index_t* index_get(index_t* idx, char* word);
```

#### `querier.c`
```c
int fileno(FILE *stream);
static void prompt(void);
static void parseArgs(int argc, char* argv[], char** pageDir, char** filename);
static counters_t* disjunctOrSequence(index_t* idx, char** wordSequence);
static counters_t* conjunctAndSequence(index_t* idx, char** sequence, int* pos, int start);
int main(int argc, char* argv[])
```

#### `word.c`
```c
char* word_normalizeWord(const char* word);
void word_normalizeSequence(char** sequence);
bool word_checkSyntax(char** sequence);
char** word_decomposeSequence(char* query);
void word_printSequence(char** wordSequence);
```

#### `scoreboard.c`
```c
scoreboard_t* scoreboard_new(counters_t* scores, int expectedSize);
void scoreboard_delete(scoreboard_t* sb);
void scoreboard_print(scoreboard_t* sb, char* pageDirectory);
```

#### `union.c`
```c
union_t* union_new();
void union_conjunction(union_t* uni, counters_t* counter);
void union_disjunction(union_t* uni, counters_t* counter);
counters_t* union_getCounter(union_t* uni);
void union_delete(union_t* uni);
void union_pointerDelete(union_t* uni);
```

## Error Handling and Recovery
### Command line errors
If the incorrect number of arguments is received, or any of the arguments are invalid, the program sends an error message and exits cleanly.

### Query Parsing Errors
Invalid characters (e.g. '!', '1', '^') and improper operator usage (e.g., "or or", "and or") are caught and reported. The program does not exit, and instead frees any memory being used in this cycle and prompts the user again.

### Memory Management:
Every allocation uses mem_malloc_assert or mem_calloc_assert, so all errors allocating memory will result in an exit.
Intermediate objects like word sequences, scores, and scoreboard are freed after each query.
On unrecoverable failure, program exits cleanly with message.

## Testing Plan
### Unit Testing
The `union` and `scoreboard` modules are pretty small, so it will be adequate to observe their behavior during the integration test of the entires system.
*word module*: We will test the word module with a its own dedicated driver. The driver will take lines of input from stdin and then pass them to `word_decomposeSequence`. It will then pass the sequence produced to `word_normalizeSequence` and `word_checkSyntax`. If any errors are detected in these functions, the driver will print `false`, otherwise it will print `true`.

### Regression Testing
As we add more modules to the querier, we will test each iteration against a predetermined sequence of queries and compare outputs between iterations

### Integration/System Testing
Write a testing.sh script that:
*runs invalid inputs*:
- Run with no arguments, one, and more than 2 arguments
- Run with invalid page directory
- Run with invalid index file
- Enter empty query
- Enter query with non alphabetic characters
- Enter query with consecutive 'and's or 'or's
- Enter query starting with 'and' or 'or'
- Enter query ending with 'and' or 'or'

*runs valid inputs*:
We will run a test with valid inputs fed in from fuzzed query files. These will contain a wide array of words and operands. We will perform these tests on the page directories for wikipedia depth 1, toscrape depth 1, and letters depth 10.

*runs memory tests*:
Confirm no leaks using valgrind.