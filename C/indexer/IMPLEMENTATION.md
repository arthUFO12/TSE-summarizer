# Implementation spec

## Data structures
The `indexer` and `indextest` modules will implement the `index` data structure. The index is a wrapper for the `hashtable` data structure. Its keys represent words in the index and its items point to `counters` data structures that keep track of the occurrences of each word within a specific document. The `indexer` module will initialize an `index` of size 200, since we can't know how many words will be entered into the index, while the `indextest` module will initialize an `index` of size equal to the number of lines in an index file.

## Modules 

### `indexer.c`
The indexer is implemented with 4 functions
#### `main`
The main function calls parseArgs, calls indexBuild to create an index, and then calls index_save to save that index to a file.

#### `parseArgs`
Given arguments from the command line, extract them into the function parameters; return only if successful. 
- for `pageDirectory`, call `pagedir_validate()`
if any trouble is found, print an error to stderr and exit non-zero.
#### `indexBuild`
Loads each page starting at ID = 1 and passes them to indexPage.
```
create a new empty index with 200 slots
set document ID to 1
while we can load a page with this ID:
    call indexPage to add words from the page into the index
    delete the loaded webpage to free memory
    increment document ID
return the built index
```
#### `indexPage`
Extracts words from the webpage and increments the count of the page for that respective word.
```
While we can extract a word from the webpage:
    if the word length is at least 3 characters:
        normalize the word
        increment the count for this word and document ID in the index
        free the normalized word
    free the original word
```

### `indextest.c`
The indextest loads an in memory index from a saved index file and then saves that index to a new file.
#### `main`
- Calls `parseArgs`
- Calls `index_reconstruct` to build index from saved file
- Calls `index_save` to save to a new file

### `index.c`
The index module provides a data structure that maps words to counters (document ID and count pairs).

#### `index_new`
Creates and returns a new index with the specified number of hashtable slots. Returns NULL if the size is invalid.

#### `index_incrementCount`
Increments the count of a word for a given document ID.
```
Create a new counter
If insertion of counter for the word is unsuccessful:
    Delete the counter
Add to the counter of the specified word for the specified ID
```

#### `index_insertCount`
Adds a specific count for a word and document ID to the index.
- If the word is already present, the count is updated.

#### `index_delete`
Deletes the index and all associated memory, including the hashtable and the internal counters for each word.

#### `index_save`
Writes the contents of the index to a file.
- Each line contains a word followed by its (ID, count) pairs in the format:
word id1 count1 id2 count2 ...
```
Open file
Call hastable_iterate on the index's hashtable and pass our print function to it
Close file
```

#### `index_reconstruct`
Reads an index from a file and reconstructs it in memory.
```
Open file
Create an index with size equal to the number of lines in the file
While we can scan a word:
    While we can scan an ID and count:
        Insert the current count at the ID in the current word
Close file
Return index
```

#### `pagedir.c`
We create 3 new functions in `pagedir.c` for the purposes of the indexer
#### `pagedir_validate`
Checks whether the given directory was produced by the crawler by verifying the existence of the `.crawler` marker file.
```
construct a path by appending '/.crawler' to the directory name
attempt to open this path as a file
    if the file cannot be opened:
        print an error to stderr and return false
    else:
        close the file and return true
```
#### `pagedir_validateWriteFile`
Checks whether the given file can be opened in write mode.
```
If file can't be opened in write mode
    return false
Close file
return true
```
#### `pagedir_load`
Given a page directory and document ID, loads the corresponding webpage file and returns a new `webpage_t` object.
```
construct a path by appending '/' and the docID to the page directory
if we can't open a file at this location:
    return false
read the first line as the URL
read the next line as the crawl depth
read the remainder of the file as the HTML content
close the file
return a new webpage object constructed from the URL, depth, and HTML
```
### `word.c`
The word module implements a function called word_normalizeWord that converts a word to lowercase letters.

### `libcs50`
We anticipate the use of the `hashtable` and `counters` modules in order to construct our `index` data structure. We also anticipate the use of the `file` and `webpage` modules in order to read from files and extract words from webpages.

## Function prototypes
Information on these files can be found in either the header or module files.
#### `indexer.c`
```c
int main(int argc, char* argv[]);
static void parseArgs(int argc, char* argv[], char** pageDirectory, char** indexFilename);
static index_t* indexBuild(const char* pageDirectory);
static void indexPage(index_t* idx, webpage_t* page, int id);
```
#### `indextest.c`
```c
int main(int argc, char* argv[]);
static void parseArgs(int argc, char* argv[], char** old, char** new);
```

#### `index.c`
```c
index_t* index_new(int size);
int index_incrementCount(index_t* idx, const char* word, int id);
bool index_insertCount(index_t* idx, const char* word, int id, int count);
void index_delete(index_t* idx);
static void idCountPrint(void* file, int id, int count);
static void wordPrint(void* file, const char* word, void* counter);
bool index_save(index_t* idx, char* filename);
index_t* index_reconstruct(char* oldFilename)
```

#### `pagedir.c`
```c
bool pagedir_validate(const char* pageDirectory);
webpage_t* pagedir_load(const char* pageDirectory, const int docID);
bool pagedir_validateWriteFile(const char* filePath);
```

#### `word.c`
```c
char* word_normalizeWord(const char* word);
```

## Error handling and recovery
For `indexer.c` the command-line parameters are checked before any data structures are allocated or work begins; problems result in a message printed to stderr and a non-zero exit status.

`indextest.c` will be more lax in its error checking, only ensuring the correct number of arguments are passed to the module. The module assumes that all files are in the correct format, and if any problems arise while trying to open the files, it exits non-zero.

Out-of-memory errors are handled by variants of the mem_assert functions, which result in a message printed to stderr and a non-zero exit status. All code uses defensive-programming tactics to catch and exit (using variants of the mem_assert functions), e.g., if a function receives bad parameters.

Errors are all handled internally in the `indexer.c` and `indextest.c` modules.

## Testing
### Unit testing
There are 4 different units. The `indexer` represents the entire system and is covered below. The `word` modules and the additions to the `pagedir` module are small and their behavior can be observed during the testing of the `indexer` module. The `indextest` program will act as a driver for the `index` module. It will reconstruct an in memory index from a saved index file and then save it to another file. These unit tests will be conducted after each system test as well as an additional run with valgrind to ensure no memory leaks

### Integration testing
The `indexer` program will be tested by constructing an in memory index data structure from files in a page directory and then saved to file. This file will then be validated by `indextest` to ensure it was saved correctly. These tests will be ran from a bash script called `testing.sh` to ensure the same routine is followed each testing cycle.
The testing routine will go as follows:

#### Testing with bad parameters
1. Test with no arguments passed
2. Test with one argument passed
3. Test with more than 2 arguments passed
4. Non-existent page directory
5. Non-crawler page directory
6. Non-existent location for indexFile
7. IndexFile in read-only directory
8. IndexFile is read-only

#### Testing over all page directories
To run tests, just run `make test`.
We run the tests over all page directories using:
```
./indexer ../data/XXXX-depth-X ../data/XXXX-depth-X/XXXX.index
```
After each run of the indexer, we call `indextest` passing it the file that `indexer` just produced like so:
```
./indextest ../data/XXXX-depth-X/XXXX.index ../data/XXXX-depth-X/XXXX.reindex
```
#### Valgrind
Then a valgrind test of `indexer` and `indextest` will be ran to ensure no memory leaks

#### Validation Testing
Optionally, we can run `make validate` to run `validate.sh`. This script will use `indexcmp`, copied from the shared directory, to compare the file produced by `indexer` and `indextest` for each file.