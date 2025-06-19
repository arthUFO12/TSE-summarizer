/*
 * union.h - header file for CS50 'union' module
 *
 * This module provides an wrapper for combining counters sets using
 * logical conjunction (AND) and disjunction (OR) operations.
 *
 * Arthur Ufongene, May 2025
 */

#include "counters.h"

typedef struct unions union_t;

/********** union_new *************/
/* Create a new union object
 *
 * We return:
 *   A pointer to a new union_t object with an empty counters set
 * Caller is responsible for:
 *   Later calling union_delete or union_pointerDelete to free memory
 */
union_t* union_new(void);

/********** union_conjunction *************/
/* Perform logical AND with the internal counter and the given counter
 *
 * Caller provides:
 *   A valid union_t*
 *   A counters_t* to combine via conjunction
 * We do:
 *   Replace the internal counter with the minimum value per key
 *   Only keys common to both counters are preserved
 */
void union_conjunction(union_t* uni, counters_t* counter);

/********** union_disjunction *************/
/* Perform logical OR with the internal counter and the given counter
 *
 * Caller provides:
 *   A valid union_t*
 *   A counters_t* to combine via disjunction
 * We do:
 *   Add values of matching keys; all keys are preserved
 */
void union_disjunction(union_t* uni, counters_t* counter);

/********** union_getCounter *************/
/* Return the internal counters set from the union
 *
 * Caller provides:
 *   A valid union_t*
 * We return:
 *   The internal counters_t* from the union
 * Caller should not modify or delete the returned pointer directly
 */
counters_t* union_getCounter(union_t* uni);

/********** union_delete *************/
/* Delete the union object and its internal counters
 *
 * Caller provides:
 *   A union_t* returned by union_new
 * We do:
 *   Free the internal counters and the union struct
 */
void union_delete(union_t* uni);

/********** union_pointerDelete *************/
/* Delete only the union struct (not its internal counters)
 *
 * Caller provides:
 *   A union_t* (typically already owned counters or in iterative deletion)
 * We do:
 *   Only free the union struct
 */
void union_pointerDelete(union_t* uni);
