/*
 * index.c - CS50 'index' module
 *
 * This module implements the index data structure, which maps words
 * to a set of (document ID, count) pairs using a hashtable where
 * each key is a word and the value is a counters structure.
 *
 * See index.h for more information.
 *
 * Arthur Ufongene, May 2025
 */

#include "hashtable.h"
#include "mem.h"
#include "index.h"
#include "file.h"

// Static function prototypes
static void idCountPrint(void* file, int id, int count);
static void wordPrint(void* file, const char* word, void* counter);

// index structure definition, contains a hashtable where each key is a word,
// and the corresponding value is a counters_t* that stores (id, count) pairs
struct index {
  hashtable_t* idxTable;
};

/************ index_new **********/
/* see index.h for more details */
index_t* index_new(int size)
{
  if (size <= 0) {
    return NULL; 
  }
  // allocate memory for a new index 
  index_t* newIndex = mem_calloc(1, sizeof(index_t));
  if (newIndex == NULL) {
    return NULL;
  }
  // create a new hashtable with the given size
  newIndex->idxTable = hashtable_new(size);
  if (newIndex->idxTable == NULL) {
    return NULL;
  }

  return newIndex;
}

/********* index_incrementCount *********/
/* see index.h for more details */
int index_incrementCount(index_t* idx, const char* word, int id)
{
  if (idx == NULL || word == NULL) {
    return 0;
  }
  // create a new counters structure to insert if word is not present
  counters_t* newCounter = counters_new();

  // attempt to insert newCounter under 'word'; if word already exists, insertion fails
  if (!hashtable_insert(idx->idxTable, word, newCounter)) {
    // clean up
    counters_delete(newCounter);
  }

  // ensure that the counter is actually in the hashtable
  // just in case of allocation errors
  counters_t* wordCounter;
  if ((wordCounter = hashtable_find(idx->idxTable, word)) == NULL) {
    return 0;
  }

  //increment the id count for the word's counters struct
  return counters_add(wordCounter, id);
}

/******* index_insertCount *********/
/* see index.h for more details */
bool index_insertCount(index_t* idx, const char* word, int id, int count)
{
  if (idx == NULL || word == NULL) {
    return false;
  }
  counters_t* newCounter = counters_new();
  // try to insert into hashtable
  if (!hashtable_insert(idx->idxTable, word, newCounter)) {
  // cleanup if insertion failed
    counters_delete(newCounter);
  }
  
  // ensure that the counter is actually in the hashtable
  // just in case of allocation errors
  counters_t* wordCounter;
  if ((wordCounter = hashtable_find(idx->idxTable, word)) == NULL) {
    return false;
  }

  // set count for id in the counters for word
  return counters_set(wordCounter, id, count);
}

/********* index_delete ***********/
/* see index.h for more details */
void index_delete(index_t* idx)
{
  if (idx != NULL) {
    // delete the hashtable, passing counters_delete to delete all internal counters
    hashtable_delete(idx->idxTable, (void (*)(void*)) counters_delete);
    free(idx);
  }
}

/********** idCountPrint ***********/
/* Prints the id and count of a counter entry to a file
 * 
 * file: pointer to a file struct
 * id: id of the current counter entry
 * count: count of current counter entry
 */
static void idCountPrint(void* file, int id, int count)
{
  FILE* fp = (FILE*) file;
  fprintf(fp, "%d %d ", id, count); // print in specified format to file
}

/*********** wordPrint ************/
/* Prints the word of an index entry to the file
 * and iterates through that entry's counter
 * 
 * file: pointer to file struct
 * word: key of the hashtable entry
 * counter: item of the hashtable entry
 */
static void wordPrint(void* file, const char* word, void* counter)
{
  FILE* fp = (FILE*) file;
  // print the word
  fprintf(fp, "%s ", word);
  // iterate through the counters for that word and print each (id, count)
  counters_iterate((counters_t*)counter, file, idCountPrint);
  fprintf(fp, "\n"); 
}

/*********** index_save ************/
/* see index.h for more details */
bool index_save(index_t* idx, char* filename)
{
  FILE* file;
  // open file for writing
  if (idx == NULL || (file = fopen(filename, "w")) == NULL) {
    return false;
  }

  // iterate through the hashtable and print each word and its counters
  hashtable_iterate(idx->idxTable, (void*) file, wordPrint);

  // close the file
  fclose(file);
  return true;
}

/*********** index_reconstruct **********/
/* see index.h for more details */
index_t* index_reconstruct(char* oldFilename)
{
  FILE* oldFile;
  // open the saved index file for reading
  if ((oldFile = fopen(oldFilename, "r")) == NULL) {
    return NULL;
  }

  // create a new index with a size based on number of lines in the file
  index_t* idx;
  if ((idx = index_new(file_numLines(oldFile))) == NULL) {
    return NULL;
  }

  // variables to hold each parsed word/id/count
  char currWord[200];
  int currId;
  int currCount;

  // read each word and its (id, count) pairs from the file
  while (fscanf(oldFile, "%s ", currWord) == 1) {
    while (fscanf(oldFile, "%d %d ", &currId, &currCount) == 2) {
      // try insert the data into the index
      // If data is not inserted return null indicating an error was made
      if (!index_insertCount(idx, currWord, currId, currCount)) {
        return NULL;
      }
    }
  }

  // close file after processing
  fclose(oldFile);
  return idx;
}

/********** index_get *************/
/* See index.h for more information */
counters_t* index_get(index_t* idx, char* word)
{
  return (counters_t*) hashtable_find(idx->idxTable, word);
}
