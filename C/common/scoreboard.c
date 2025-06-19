
/*
 * scoreboard.c - CS50 'scoreboard' module
 *
 * This module implements a scoreboard data structure used for ranking
 * scored document IDs based on their query score. The scoreboard is
 * essentially a pointer to a calloc'd array of scoreEntry_t*'s sorted
 * by C's qsort function.
 *
 * Arthur Ufongene, May 2025
 */

#include "scoreboard.h"
#include "mem.h"
#include "pagedir.h"

typedef struct scoreEntry scoreEntry_t;

// Internal structure to store document ID and its score
struct scoreEntry {
  int id;
  int score;
};

// Internal structure for the scoreboard
struct scoreboard {
  int place;               // current index for iteration
  int size;                // total number of entries
  scoreEntry_t** board;    // array of pointers to score entries
};

// Internal helper function prototypes
static int sortFunc(const void* firstAddress, const void* secondAddress);
static void addToBoard(void* address, int id, int count);
static scoreEntry_t* scoreEntry_new(int id, int score);
static void scoreEntry_delete(scoreEntry_t* entry);
static bool next(scoreboard_t* sb);
static int getId(scoreboard_t* sb);
static int getScore(scoreboard_t* sb);

/********** scoreboard_new *************/
/* See scoreboard.h for more information */
scoreboard_t* scoreboard_new(counters_t* scores, int expectedSize)
{
  // allocate memory for scoreboard
  scoreboard_t* newBoard = mem_calloc_assert(1, sizeof(scoreboard_t), "No space");

  // allocate memory for array of score entries
  newBoard->board = mem_calloc_assert(expectedSize, sizeof(scoreEntry_t*), "No space for scores array");

  newBoard->place = -1;

  // pointer to the beginning of the array
  scoreEntry_t** pos = newBoard->board;

  // iterate over all counters, adding their entries to the array and moving the pos pointer
  counters_iterate(scores, &pos, addToBoard);

  // calculate actual number of entries
  newBoard->size = pos - newBoard->board;

  // sort entries by score in descending order using the qsort function
  qsort(newBoard->board, newBoard->size, sizeof(scoreEntry_t*), sortFunc);
  
  return newBoard;
}

/********** scoreboard_print *************/
/* See scoreboard.h for more information */
void scoreboard_print(scoreboard_t* sb, char* pageDirectory) 
{
  // If the scoreboard is empty, print no matches
  if (sb->size == 0) {
    printf("No matches found\n\n");
    fflush(stdout);
    return;
  }
  char* URL;
  printf("%d matches ranked below\n", sb->size);

  // iterate and print each entry
  while (next(sb)) {

  // get URL from the page with this id, then print its score, id, and URL
    URL = pagedir_getURL(pageDirectory, getId(sb));
    printf("Score %3d | Doc %3d: %s\n", getScore(sb), getId(sb), URL);
    free(URL);
  }
  printf("\n");
  fflush(stdout);
  // reset for reuse
  sb->place = -1;
}

/********** scoreboard_delete *************/
/* see scoreboard.h for more information */
void scoreboard_delete(scoreboard_t* sb)
{

  // free each score entry
  while (next(sb)) {
    scoreEntry_delete(sb->board[sb->place]);
  }

  // free array and scoreboard struct
  free(sb->board);
  free(sb);
}

/********** next *************/
/* Advance the scoreboard iterator and return true if next exists */
static bool next(scoreboard_t* sb)
{
  return sb->board[++(sb->place)] != NULL;
}

/********** getId *************/
/* Return the document ID at current position */
static int getId(scoreboard_t* sb)
{
  return (sb->board[sb->place])->id;
}

/********** getScore *************/
/* Return the score at current position */
static int getScore(scoreboard_t* sb)
{
  return (sb->board[sb->place])->score;
}

/********** sortFunc *************/
/* Sorting comparator for qsort: highest score first */
static int sortFunc(const void* firstAddress, const void* secondAddress)
{
  // dereference the pointers and subtract the first from the second
  scoreEntry_t* score1 = *((scoreEntry_t**) firstAddress);
  scoreEntry_t* score2 = *((scoreEntry_t**) secondAddress);
  return score2->score - score1->score;
}

/********** addToBoard *************/
/* Add a new scoreEntry to the board array
 *
 * Called by counters_iterate with:
 *   address: a pointer to a scoreEntry_t** pointer into board array
 *   id: docID
 *   count: score for that docID
 */
static void addToBoard(void* address, int id, int count)
{
  // pointer to a pointer to the current score entry in the array
  scoreEntry_t*** scoreAddress = (scoreEntry_t***) address;

  // make sure we only write in calloc'd space
  if (**scoreAddress == NULL) {
    // make a new score entry at this spot in the array
    **scoreAddress = scoreEntry_new(id, count);

    // increment the spot in the array so the next entry can be added
    (*scoreAddress)++;
  }
}

/********** scoreEntry_new *************/
/* Create a new score entry with given id and score */
static scoreEntry_t* scoreEntry_new(int id, int score) 
{
  scoreEntry_t* newEntry = mem_calloc_assert(1, sizeof(scoreEntry_t), "No space");
  newEntry->id = id;
  newEntry->score = score;
  return newEntry;
}

/********** scoreEntry_delete *************/
/* Free memory associated with a scoreEntry */
static void scoreEntry_delete(scoreEntry_t* entry) 
{
  free(entry);
}
