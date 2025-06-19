/*
 * scoreboard.h - header file for CS50 'scoreboard' module
 *
 * This module provides a scoreboard data structure that stores and ranks
 * scored document IDs from a counters data structure.
 *
 * Arthur Ufongene, May 2025
 */

#include "counters.h"

typedef struct scoreboard scoreboard_t;

/********** scoreboard_new *************/
/* Create a new scoreboard from a counters set
 *
 * Caller provides:
 *   A counters_t* mapping docIDs to scores
 *   An int size representing the maximum expected number of scores
 * We return:
 *   A pointer to a new scoreboard_t struct containing sorted entries
 * Caller is responsible for:
 *   Later calling scoreboard_delete to free memory
 */
scoreboard_t* scoreboard_new(counters_t* scores, int expectedSize);

/********** scoreboard_delete *************/
/* Delete the given scoreboard and free memory
 *
 * Caller provides:
 *   A pointer to a scoreboard_t returned from scoreboard_new
 * We do:
 *   Free all internal memory, including the scoreboard and its entries
 */
void scoreboard_delete(scoreboard_t* sb);

/********** scoreboard_print *************/
/* Print the contents of the scoreboard in descending order
 *
 * Caller provides:
 *   A valid scoreboard_t*
 *   A valid pageDirectory (path to page files for URL lookup)
 * We print:
 *   If size == 0: nothing; else:
 *   Each document's score, ID, and URL retrieved via pageDirectory
 */
void scoreboard_print(scoreboard_t* sb, char* pageDirectory);
