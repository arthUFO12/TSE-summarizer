/*
 * union.c - CS50 'union' module
 *
 * This module implements an abstraction for computing logical AND (conjunction)
 * and OR (disjunction) between sets of document scores using counters.
 * 
 *
 * Arthur Ufongene, May 2025
 */

#include <stdlib.h>
#include "union.h"
#include "mem.h"
#include "bag.h"

// Internal structure for union object
struct unions {
  counters_t* counter;
};

// Internal function prototypes
static int union_get(union_t* uni, int id);
static bool union_set(union_t* uni, int id, int count);
static void disjunct(void* uni, int id, int count);
static void addToBag(void* bag, int id, int count);

/********** union_new *************/
/* See union.h for more information */
union_t* union_new(void) 
{
  union_t* newUnion = mem_calloc_assert(1, sizeof(union_t), "Couldn't allocate union");
  newUnion->counter = counters_new();
  return newUnion;
}

/********** union_conjunction *************/
/* See union.h for more information */
void union_conjunction(union_t* uni, counters_t* counter)
{
  counters_t* conjunction = counters_new();      // result of conjunction will be in here
  bag_t* counterEntries = bag_new();             // bag of docID's that are in the counter

  // Extract all docIDs from the input counter
  counters_iterate(counter, counterEntries, addToBag);

  int* currId;
  int unionCount;
  int counterCount;

  // Compare values for shared keys
  while ((currId = bag_extract(counterEntries)) != NULL) {
    // if the docID is in the union
    if ((unionCount = union_get(uni, *currId)) > 0) {

    // add the score of whichever is lower
      if ((counterCount = counters_get(counter, *currId)) < unionCount) {
        counters_set(conjunction, *currId, counterCount);
      } else {
        counters_set(conjunction, *currId, unionCount);
      }
    }
    // free calloc'd int*
    free(currId);
  }
  
  // clean up and replace old with new
  bag_delete(counterEntries, NULL);
  counters_delete(uni->counter); 

  uni->counter = conjunction;
}

/********** union_disjunction *************/
/* See union.h for more information */
void union_disjunction(union_t* uni, counters_t* counter)
{
  // call disjunct to add all entries to the union
  counters_iterate(counter, uni, disjunct);
}

/********** union_getCounter *************/
/* See union.h for more information */
counters_t* union_getCounter(union_t* uni)
{
  return uni->counter;
}

/********** union_delete *************/
/* See union.h for more information */
void union_delete(union_t* uni) 
{
  counters_delete(uni->counter);
  free(uni);
}

/********** union_pointerDelete *************/
/* see union.h for more information */
void union_pointerDelete(union_t* uni)
{
  free(uni);
}

/********** union_get *************/
/* Retrieves the score associated with the 
 * id in the counter of a union
 */
static int union_get(union_t* uni, int id)
{
  return counters_get(uni->counter, id);
}

/********** union_set *************/
/* Sets the score of an entry in the counter
 * data structure of a union
 */
static bool union_set(union_t* uni, int id, int count)
{
  return counters_set(uni->counter, id, count);
}

/********** disjunct *************/
/* Helper for disjunction
 * adds the values of entries in a counter
 * to the values of corresponding entries in the union
 */
static void disjunct(void* uni, int id, int count) 
{
                               // get current value and add the value of the counter
  union_set((union_t*) uni, id, union_get((union_t*) uni, id) + count);
}

/********** addToBag *************/
/* Copy ID into bag to allow iteration */
static void addToBag(void* bag, int id, int count)
{
  // store the id in an int and put it into the bag
  int* idCopy = mem_malloc_assert(sizeof(int), "Couldn't add id to bag");
  *idCopy = id;
  bag_insert((bag_t*) bag, idCopy);
}
