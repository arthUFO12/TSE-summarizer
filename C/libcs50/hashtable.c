/*
 * hashtable.c - CS50 'hashtable' module
 *
 * See hashtable.h for more implementation
 * This implementation uses an array of set_t* for buckets.
 *
 * Arthur Ufongene, April 2025
 */

#include <stdlib.h>
#include <string.h>
#include "mem.h"
#include "set.h"
#include "hash.h"
#include "hashtable.h"

/**************** local types ****************/ 
struct hashtable {
  int table_size;           // number of slots in table
  set_t** table;            // array of set pointers
};

/**************** global functions ****************/

/**************** hashtable_new() ****************/
// See hashtable.h for description
hashtable_t* hashtable_new(const int num_slots)
{
  if (num_slots <= 0) return NULL; // invalid size
                                   // exit if errors with allocation
  hashtable_t* new_hashtable = mem_calloc_assert(1, sizeof(hashtable_t), "couldn't allocate for hashtable\n");
  new_hashtable->table = mem_calloc_assert(num_slots, sizeof(set_t*), "couldn't allocate for table\n");
  new_hashtable->table_size = num_slots;
  return new_hashtable;
}

/**************** hashtable_insert() ****************/
// See hashtable.h for description
bool hashtable_insert(hashtable_t* ht, const char* key, void* item)
{
  if (ht == NULL || key == NULL || item == NULL) { // check for null arguments
    fprintf(stderr, "Attempted to insert on null hashtable or with null parameters\n");
    return false;
  }
  int idx = hash_jenkins(key, ht->table_size);     // compute hash index

  if (ht->table[idx] == NULL) {
    ht->table[idx] = set_new();                    // create new set if necessary
  }
  return set_insert(ht->table[idx], key, item);    // insert into set
}

/**************** hashtable_find() ****************/
// See hashtable.h for description
void* hashtable_find(hashtable_t* ht, const char* key)
{
  if (ht == NULL || key == NULL) { // check for null parameters
    fprintf(stderr, "Attempted to find on null hashtable or with null key\n");
    return NULL;
  }                                // obtain slot to search
  set_t* slot = ht->table[hash_jenkins(key, ht->table_size)];
  if (slot == NULL) {
    return NULL;
  }
  return set_find(slot, key);      // search in that slot
}

/**************** hashtable_iterate() ****************/
// See hashtable.h for description
void hashtable_iterate(hashtable_t* ht, void* arg, void (*itemfunc)(void* arg, const char* key, void* item))
{
  if (ht == NULL) {                          // check for null hashtable
    fprintf(stderr, "Attempted to iterate on null hashtable or with null itemfunc\n");
    return;
  }
  for (int i = 0; i < ht->table_size; i++) { // loop through all slot in hashtable
    if (ht->table[i] != NULL) {
      set_iterate(ht->table[i], arg, itemfunc); // iterate over each set
    }
  }
}

/**************** hashtable_print() ****************/
// See hashtable.h for description
void hashtable_print(hashtable_t* ht, FILE* fp, void (*itemprint)(FILE* fp, const char* key, void* item))
{
  if (fp == NULL) {           // Check for null arguments
    fprintf(stderr, "Attempted to print with null arguments");
    return;
  }
  if (ht == NULL) {
    fprintf(fp, "(null)\n");
    return;
  }
  if (itemprint == NULL) {
    for (int i = 0; i < ht->table_size; i++) printf("\n");
    return;
  }
  for (int i = 0; i < ht->table_size; i++) { 
    fprintf(fp, "%d ", i+1);  // print the slot number of each set
    set_print(ht->table[i], fp, itemprint); // print each set
  }
}

/**************** hashtable_delete() ****************/
// See hashtable.h for description
void hashtable_delete(hashtable_t* ht, void (*itemdelete)(void* item))
{
  if (ht == NULL) {                         // check for null hashtable
    fprintf(stderr, "Attempted to delete on null hashtable or with null itemdelete\n");
    return;
  }
  for (int i = 0; i < ht->table_size; i++) {// loop through all slots
    if (ht->table[i] != NULL) {
      set_delete(ht->table[i], itemdelete); // delete each set
    }
  }
  mem_free(ht->table); // free table array
  mem_free(ht); // free hashtable structure
}


