
/*
 * indexer.c - CS50 'indexer' module
 *
 * This module builds an inverted index from a set of webpages
 * stored in a given directory (output of the crawler), and saves
 * the index to a specified file for later searching.
 *
 * Arthur Ufongene, May 2025
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mem.h"
#include "index.h"
#include "string.h"
#include "pagedir.h"
#include "file.h"
#include "word.h"

// Function prototypes
int main(int argc, char* argv[]);
static void parseArgs(int argc, char* argv[], char** pageDirectory, char** indexFilename);
static index_t* indexBuild(const char* pageDirectory);
static void indexPage(index_t* idx, webpage_t* page, int id);

/**************** main ****************/
/* Parses arguments, builds index from a given directory,
 * and saves it to a file.
 */
int main(int argc, char* argv[])
{
  char* indexFilename;
  char* pageDirectory;

  // Parse command-line arguments to retrieve pageDirectory and indexFilename
  parseArgs(argc, argv, &pageDirectory, &indexFilename);

  // Build the index from the pageDirectory
  index_t* pageIdx;
  if ((pageIdx = indexBuild(pageDirectory)) == NULL) {
    fprintf(stderr, "Couldn't build index\n");
    exit(-1);
  }

  // Save the index to the specified file
  if (!index_save(pageIdx, indexFilename)) {
    fprintf(stderr, "Couldn't open file\n");
    exit(-1);
  }

  // Clean up memory
  index_delete(pageIdx);
  free(indexFilename);
  free(pageDirectory);

  exit(0);
}

/**************** parseArgs ****************/
/* Validates and parses command-line arguments.
 * 
 * argc: number of arguments
 * argv: argument vector
 * pageDirectory: pointer to store the directory of webpages
 * indexFilename: pointer to store the output index file name
 */
static void parseArgs(int argc, char* argv[], char** pageDirectory, char** indexFilename)
{
  if (argc != 3) {
    fprintf(stderr, "Incorrect number of arguments\n");
    exit(-1);
  }
  
  // Ensure the directory was created by the crawler
  if (!pagedir_validate(argv[1])) {
    fprintf(stderr, "Not a crawler directory\n");
    exit(-1);
  }

  // Make sure that the file can be written to
  if (!pagedir_validateWriteFile(argv[2])) {
    fprintf(stderr, "Couldn't open indexFile\n");
    exit(-1);
  }

  // Allocate space for and copy pageDirectory
  *pageDirectory = mem_calloc_assert(strlen(argv[1]) + 1, sizeof(char), "Couldn't assign space for pageDirectory\n");
  strcpy(*pageDirectory, argv[1]);

  // Allocate space for and copy indexFilename
  *indexFilename = mem_calloc_assert(strlen(argv[2]) + 1, sizeof(char), "No space for file name\n");
  strcpy(*indexFilename, argv[2]);
}

/**************** indexBuild ****************/
/* Builds an index from all webpages found in the given pageDirectory.
 * 
 * pageDirectory: directory containing crawler-produced HTML files
 *
 * Returns: a pointer to the built index
 */
static index_t* indexBuild(const char* pageDirectory)
{
  // Create a new index with a reasonable number of slots
  index_t* idx;
  if ((idx = index_new(200)) == NULL) {
    return NULL;
  }
  
  int id = 1;

  webpage_t* page;

  // Load each page one-by-one from the directory until no more pages exist
  while ((page = pagedir_load(pageDirectory, id)) != NULL) {
    // Index the words in the current page
    indexPage(idx, page, id);
    // Clean up the page memory
    webpage_delete(page);
    id++;
  }

  if (id == 1) {
    return NULL;
  }

  return idx;
}

/**************** indexPage ****************/
/* Processes a single webpage and adds its words to the index.
 * 
 * idx: the index to add words into
 * page: the webpage to process
 * id: the document ID for this page
 */
static void indexPage(index_t* idx, webpage_t* page, int id)
{
  char* word;
  int pos = 0;

  // Extract words one-by-one from the page
  while ((word = webpage_getNextWord(page, &pos)) != NULL) {
    // Only index words with length >= 3
    if (strlen(word) >= 3) {
      // Normalize the word (e.g., lowercase)
      char* normalized = word_normalizeWord(word);
      // Add or increment the word count in the index for this document ID
      index_incrementCount(idx, normalized, id);
      free(normalized);
    }
    free(word);
  }
}
