
/*
 * set.c - CS50 'set' module
 *
 * See set.h for more information
 * This implementation uses a binary search tree.
 *
 * Arthur Ufongene, April 2025
 */

#include <stdlib.h>
#include <string.h>
#include "mem.h"
#include "set.h"

/**************** file-local types ****************/ 
typedef struct set_entry set_entry_t;

struct set_entry {
  char* key;                  // key for this entry
  void* item;                  // item associated with key
  set_entry_t* left;            // left child in binary tree
  set_entry_t* right;           // right child in binary tree
};


/****************** global types *******************/
struct set {
  set_entry_t* root;            // root of the binary search tree
};

/**************** file-local functions ****************/
static set_entry_t* set_entry_new(const char* key, void* item); // create a new set entry
static void* set_find_entry(set_entry_t* current_entry, const char* key); // recursive find helper
static bool insert_new_entry(set_entry_t* current_entry, const char* key, void* item); // recursive insert helper
static void set_entry_delete(set_entry_t* entry, void (*itemdelete)(void* item)); // recursive delete helper
static bool hasLeft(set_entry_t* entry); // check if entry has left child
static bool hasRight(set_entry_t* entry); // check if entry has right child
static void entry_iterate(set_entry_t* entry, void* arg, void (*itemfunc)(void* arg, const char* key, void* item)); // recursive iterate helper
static void entry_print(set_entry_t* entry, set_entry_t* root, FILE* fp, void (*itemprint)(FILE* arg, const char* key, void* item)); // recursive print helper

/**************** global functions ****************/

/**************** set_new() ****************/
/* See set.h for description */
set_t* set_new(void)
{
  // exit if errors with allocation
  return (set_t*) mem_calloc_assert(1, sizeof(set_t), "Memory for new set could not be allocated\n");
}

/**************** set_entry_new() ****************/
/* Allocate and initialize a new set_entry */
static set_entry_t* set_entry_new(const char* key, void* item)
{
                               // exit if errors with allocation
  set_entry_t* new_entry = (set_entry_t*) mem_calloc_assert(1, sizeof(set_entry_t), "Memory for new set entry could not be allocated\n");
  new_entry->key = (char*) mem_calloc_assert(strlen(key) + 1 , sizeof(char), "Memory for key could not be allocated\n");
  strcpy(new_entry->key, key); // copy key
  new_entry->item = item;      // set item
  return new_entry;
}

/**************** set_insert() ****************/
/* See set.h for description */
bool set_insert(set_t* set, const char* key, void* item)
{
  if (set == NULL || key == NULL || item == NULL) { // check null parameters
    fprintf(stderr, "Set or key is null\n");
    return false;
  }
  if (set->root == NULL) {
    set->root = set_entry_new(key, item); // Insert at root if tree is empty
    return true;
  }
  return insert_new_entry(set->root, key, item); // otherwise recursively insert
}

/**************** insert_new_entry() ****************/
/* Helper to recursively insert new entries into the BST */
static bool insert_new_entry(set_entry_t* current_entry, const char* key, void* item)
{
  int cmp = strcmp(key, current_entry->key);
  if (cmp == 0) { // Duplicate key
    return false;
  } else if (cmp > 0) { // Go right
    if (hasRight(current_entry)) {
      return insert_new_entry(current_entry->right, key, item); // recursively call if right entry
    } else {
      current_entry->right = set_entry_new(key, item); // else insert right
      return true;
    }
  } else { // Go left
    if (hasLeft(current_entry)) {
      return insert_new_entry(current_entry->left, key, item); // same for left
    } else {
      current_entry->left = set_entry_new(key, item);
      return true;
    }
  }
}

/**************** set_find() ****************/
/* See set.h for description */
void* set_find(set_t* set, const char* key)
{
  if (set == NULL || key == NULL) { // check null parameters
    fprintf(stderr, "Set or key is null\n");
    return NULL;
  }
  if (set->root == NULL) {
    return NULL; // empty tree
  }
  return set_find_entry(set->root, key); // search recursively
}

/**************** set_find_entry() ****************/
/* Recursive helper to find an item by key */
static void* set_find_entry(set_entry_t* current_entry, const char* key)
{
  int cmp = strcmp(key, current_entry->key);
  if (cmp == 0) {
    return current_entry->item; // found the key
  } else if (cmp > 0) {
    if (hasRight(current_entry)) {
      return set_find_entry(current_entry->right, key); // go right
    }
    return NULL; // not found
  } else {
    if (hasLeft(current_entry)) {
      return set_find_entry(current_entry->left, key); // go left
    }
    return NULL; // not found
  }
}

/**************** set_print() ****************/
/* See set.h for description */
void set_print(set_t* set, FILE* fp, void (*itemprint)(FILE* arg, const char* key, void* item))
{
  if (fp == NULL) {
    fprintf(stderr, "file is null\n"); // null file
    return;
  }
  if (set == NULL) {
    fprintf(fp, "(null)\n"); // null set
    return;
  }
  fprintf(fp, "{"); // print empty brackets if root or itemprint are null, else call helper
  if (set->root != NULL && itemprint != NULL) entry_print(set->root, set->root, fp, itemprint); // print entries
  fprintf(fp, "}\n");
}

/**************** entry_print() ****************/
/* Recursive helper to print entries */
static void entry_print(set_entry_t* entry, set_entry_t* root, FILE* fp, void (*itemprint)(FILE* arg, const char* key, void* item))
{
  if (entry != root) fprintf(fp, ", ");   // separate entries
  itemprint(fp, entry->key, entry->item); // print this entry
  if (hasLeft(entry)) {
    entry_print(entry->left, root, fp, itemprint); // recursively call on left
  }
  if (hasRight(entry)) {
    entry_print(entry->right, root, fp, itemprint);// recursively call on right
  }
}

/**************** set_iterate() ****************/
/* See set.h for description */
void set_iterate(set_t* set, void* arg, void (*itemfunc)(void* arg, const char* key, void* item))
{
  if (set == NULL || itemfunc == NULL) {     // check for nulls
    fprintf(stderr, "Set or itemfunc is null\n");
    return;
  }
  if (set->root != NULL) {
    entry_iterate(set->root, arg, itemfunc); // recursively iterate
  }
}

/**************** entry_iterate() ****************/
/* Recursive helper to iterate over entries */
static void entry_iterate(set_entry_t* entry, void* arg, void (*itemfunc)(void* arg, const char* key, void* item))
{ 
  if (hasLeft(entry)) {
    entry_iterate(entry->left, arg, itemfunc); // call recursively on left entry
  }
  itemfunc(arg, entry->key, entry->item);      // call itemfunc on this entry
  if (hasRight(entry)) {
    entry_iterate(entry->right, arg, itemfunc);// call recursively on right entry
  }
}

/**************** set_delete() ****************/
/* See set.h for description */
void set_delete(set_t* set, void (*itemdelete)(void* item)) 
{
  if (set == NULL) { // if null do nothing
    fprintf(stderr, "Attempted to delete null set\n");
    return;
  }
  if (set->root != NULL) {
    set_entry_delete(set->root, itemdelete); // recursively delete entries
    mem_free(set->root);                     // free the root
  }
  mem_free(set);                             // free the set itself
}

/**************** set_entry_delete() ****************/
/* Recursive helper to delete entries */
static void set_entry_delete(set_entry_t* entry, void (*itemdelete)(void* item))
{
  mem_free(entry->key); // free key
  if (itemdelete != NULL) itemdelete(entry->item); // delete item if function provided
  if (hasLeft(entry)) {
    set_entry_delete(entry->left, itemdelete);     // recursively call on left entry
    mem_free(entry->left);                         // then free left entry
  }
  if (hasRight(entry)) {
    set_entry_delete(entry->right, itemdelete);    // same for right
    mem_free(entry->right);
  }
}

/**************** hasLeft() ****************/
/* Check if entry has a left child */
static bool hasLeft(set_entry_t* entry)
{
  return (entry->left != NULL);
}

/**************** hasRight() ****************/
/* Check if entry has a right child */
static bool hasRight(set_entry_t* entry)
{
  return (entry->right != NULL);
}
