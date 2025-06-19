/* 
 * querier.c - CS50 'querier' module
 * 
 * This program reads user queries from standard input, parses and validates them,
 * retrieves matching documents using a given index, and displays results ranked
 * by score using a scoreboard.
 *
 * It supports logical "and" and "or" operators, with "and" taking precedence.
 *
 * Usage: ./querier pageDirectory indexFilename
 *
 * Arthur Ufongene, May 2025
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "file.h"
#include "mem.h"
#include "counters.h"
#include "union.h"
#include "index.h"
#include "pagedir.h"
#include "scoreboard.h"
#include "word.h"
#include <unistd.h>  // add this to your list of includes


// Function declarations
int fileno(FILE *stream);
static void prompt(void);
static void parseArgs(int argc, char* argv[], char** pageDir, char** filename);
static counters_t* disjunctOrSequence(index_t* idx, char** wordSequence);
static counters_t* conjunctAndSequence(index_t* idx, char** sequence, int* pos, int start);
int main(int argc, char* argv[]);

/********** main **********/
/* Driver function for the querier program.
 * 
 * Validates arguments, loads the index, and repeatedly prompts the user
 * for queries, processing and scoring them based on indexed documents.
 */
int main(int argc, char* argv[])
{
  char* pageDirectory;
  char* indexFilename;
                                                   // parse arguments
  parseArgs(argc, argv, &pageDirectory, &indexFilename);

  index_t* idx = index_reconstruct(indexFilename); // Load the index

  char* query;
  char** wordSequence;
  counters_t* scores;
  scoreboard_t* board;

  prompt();                                // read from stdin until EOF is received
  while ((query = file_readLine(stdin)) != NULL) {
    // Break the query into words, prompt the user again if null return
    if ((wordSequence = word_decomposeSequence(query)) == NULL) {
      free(query);
      prompt();
      continue;
    }
    // If query is empty, do nothing
    if (wordSequence[0] == NULL) {
      free(query);
      free(wordSequence);
      continue;
    }

    // Normalize the sequence of words and then check the syntax
    word_normalizeSequence(wordSequence);
    if (!word_checkSyntax(wordSequence)) {
      free(wordSequence);
      free(query);

      // prompt user again if incorrect syntax
      prompt();
      continue;
    }
      // Echo normalized sequence
    word_printSequence(wordSequence); 

      // Create a counter of scores from the sequence
    scores = disjunctOrSequence(idx, wordSequence);

      // Create a sorted scoreboard from the scores
    board = scoreboard_new(scores, 100);
      
      // Print the scoreboard
    scoreboard_print(board, pageDirectory);

      // Clean up after each query
    scoreboard_delete(board);
    counters_delete(scores);
    free(wordSequence);
    free(query);

    prompt();
  }

    // Final cleanup after EOF reached
  printf("\n");
  index_delete(idx);
  free(pageDirectory);
  free(indexFilename);
  exit(0);
}


/********** parseArgs **********/
/* Validates command-line arguments and copies values into allocated memory.
 *
 * Caller provides:
 *   argc, argv from main
 * We return:
 *   Nothing, but store allocated pageDir and filename pointers
 * If invalid, exits the program with an error.
 */
static void parseArgs(int argc, char* argv[], char** pageDir, char** filename)
{
  // validate that there are 3 arguments, the pagedir is valid, and the index file is readable
  if (argc != 3) {
    fprintf(stderr, "Wrong number of arguments\n");
    exit(-1);
  }

  if (!pagedir_validate(argv[1])) {
    fprintf(stderr, "Invalid page directory\n");
    exit(-1);
  }
  
  if (!pagedir_validateReadFile(argv[2])) {
    fprintf(stderr, "Invalid index file\n");
    exit(-1);
  }
  
  // Now check that we can read pageDir/1
  char* firstPage = mem_calloc_assert(strlen(argv[1]) + 4, sizeof(char), "Couldn't allocate space for page directory");
  strcpy(firstPage, argv[1]);
  sprintf(firstPage+strlen(argv[1]), "/%d",1);

  if (!pagedir_validateReadFile(firstPage)) {
    fprintf(stderr, "Couldn't read first file\n");
    free(firstPage);
    exit(-1);
  }

  free(firstPage);

  // Copy arguments to variables
  *pageDir = mem_calloc_assert(strlen(argv[1]) + 1, sizeof(char), "Couldn't allocate space for page directory");
  strcpy(*pageDir, argv[1]);

  *filename = mem_calloc_assert(strlen(argv[2]) + 1, sizeof(char), "Couldn't allocate space for index filename");
  strcpy(*filename, argv[2]);
}


/********** disjunctOrSequence **********/
/* Disjuncts scores across 'or's in the sequence
 *
 * Caller provides:
 *   index_t* idx - the reconstructed index
 *   char** wordSequence - validated array of query words
 * We return:
 *   counters_t* scores - combined scores across all query segments
 * Caller is responsible for:
 *   Deleting the returned counters
 */
static counters_t* disjunctOrSequence(index_t* idx, char** wordSequence)
{
  union_t* uni = union_new();    // empty union that we will perform disjunctions on
  int pos = 0;                   // keeps track of where we are in the sequence
 

  while (wordSequence[pos] != NULL) {
    if (strcmp(wordSequence[pos], "or") == 0) {
      pos++;                     // Skip over any 'or's 
    }
                                 // Get the result of the and sequence
    counters_t* andSequenceResult = conjunctAndSequence(idx, wordSequence, &pos, pos);
                                 // Make the union a disjunction of itself and the and sequence result
    union_disjunction(uni, andSequenceResult);  
                                 // Delete the and sequence
    counters_delete(andSequenceResult);

                       
  }

                                 // Retrieve the counter from the union and delete the structure
  counters_t* scores = union_getCounter(uni);
  union_pointerDelete(uni);

  return scores;
}


/********** conjunctAndSequence **********/
/* Processes a conjunctive sequence (ANDs) of words from the query.
 *
 * Caller provides:
 *   index_t* idx - the index to query
 *   char** sequence - the full word sequence
 *   int* pos - pointer to the current word index 
 *   int start - original position where this segment started
 * We return:
 *   A counters_t object of the document ID's and associated scores
 * Caller is responsible for:
 *   Deleting the returned counters
 */
static counters_t* conjunctAndSequence(index_t* idx, char** sequence, int* pos, int start)
{
  union_t* uni = union_new();
  
  // Read sequence until we encounter end of sequence or an 'or'
  for (; sequence[*pos] != NULL && strcmp(sequence[*pos], "or") != 0; (*pos)++) {
    if (strcmp(sequence[*pos], "and") == 0) {
      continue; // Skip explicit AND
    }

    if ((*pos) == start) {
      // First word: simply add its counter
      union_disjunction(uni, index_get(idx, sequence[*pos]));
    } else {
      // Subsequent words: apply AND
      union_conjunction(uni, index_get(idx, sequence[*pos]));
    }
  }
  
  counters_t* ctr = union_getCounter(uni);
  union_pointerDelete(uni);

       // return counter
  return ctr;
}


/********* prompt ***********/
/* From lab assignment readme
 * prompts only if a keyboard input
 */
static void prompt(void)
{
  // print a prompt iff stdin is a keyboard
  if (isatty(fileno(stdin))) {
    printf("Query? ");
  }
}
