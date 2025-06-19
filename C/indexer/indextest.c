/*
 * indexer-copy.c - CS50 'indexer-copy' module
 *
 * This module reads an index file previously produced by the indexer,
 * reconstructs the in-memory index data structure from it,
 * and writes it out to a new index file.
 * 
 * Arthur Ufongene, May 2025
 */

#include <stdlib.h>
#include <stdio.h>
#include "mem.h"
#include "index.h"
#include "string.h"

// Function prototypes
int main(int argc, char* argv[]);
static void parseArgs(int argc, char* argv[], char** old, char** new);

/**************** main ****************/
/* Parses arguments, reconstructs the index from a file,
 * and writes it out to a new file.
 */
int main(int argc, char* argv[])
{
  char* oldIdxFilename;
  char* newIdxFilename;

  // Parse command-line arguments to get input and output filenames
  parseArgs(argc, argv, &oldIdxFilename, &newIdxFilename);

  // Reconstruct the index from the original index file
  index_t* idx;
  if ((idx = index_reconstruct(oldIdxFilename)) == NULL) {
    fprintf(stderr, "Couldn't reconstruct index\n");
    free(oldIdxFilename);
    free(newIdxFilename);
    exit(-1);
  }
  
  // Save the reconstructed index to the new index file
  if (!index_save(idx, newIdxFilename)) {
    fprintf(stderr, "Couldn't save file\n");
    index_delete(idx);
    free(oldIdxFilename);
    free(newIdxFilename);
    exit(-1);
  }

  // Clean up memory
  index_delete(idx);
  free(newIdxFilename);
  free(oldIdxFilename);

  exit(0);
}

/**************** parseArgs ****************/
/* Validates and parses command-line arguments.
 *
 * argc: number of arguments
 * argv: argument vector
 * old: pointer to store the original index filename
 * new: pointer to store the new index filename
 */
static void parseArgs(int argc, char* argv[], char** old, char** new)
{
  if (argc != 3) {
    fprintf(stderr, "Wrong number of arguments\n");
    exit(-1);
  }

  // Allocate space for and copy the old index filename
  *old = mem_calloc_assert(strlen(argv[1]) + 1, sizeof(char), "Couldn't allocate space for old filename");
  strcpy(*old, argv[1]);

  // Allocate space for and copy the new index filename
  *new = mem_calloc_assert(strlen(argv[2]) + 1, sizeof(char), "Couldn't allocate space for new filename\n");
  strcpy(*new, argv[2]);
}
