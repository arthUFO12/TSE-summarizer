/*
 * counters.c - CS50 'counters' module
 *
 * See counters.h for details.
 *
 * Arthur Ufongene, April 2025
 */

#include <stdlib.h>
#include <string.h>
#include "mem.h"
#include "counters.h"

/**************** file-local types *********************/
typedef struct counters_entry counters_entry_t;

struct counters_entry {
  int key;                    // key for this entry
  int count;                  // count associated with key
  counters_entry_t* left;      // left child in binary tree
  counters_entry_t* right;     // right child in binary tree
};

/**************** global types ***************************/
struct counters {
  counters_entry_t* root;      // root of the binary search tree
};

/**************** file-local declarations ****************/
static void entry_print(counters_entry_t* entry, counters_entry_t* root, FILE* fp);
static bool counters_set_entry(counters_entry_t* current_entry, const int key, const int count);
static void entry_iterate(counters_entry_t* entry, void* arg, void (*itemfunc)(void* arg, const int key, const int count));
static counters_entry_t* counters_entry_new(const int key);
static int counters_get_entry(counters_entry_t* current_entry, const int key);
static int add_new_entry(counters_entry_t* current_entry, counters_entry_t* entry_to_add);
static void counters_entry_delete(counters_entry_t* entry);
static bool hasLeft(counters_entry_t* entry);
static bool hasRight(counters_entry_t* entry);

/**************** global functions ****************/
/* See counters.h for comments about exported functions */

/**************** counters_print() ****************/
void counters_print(counters_t* ctrs, FILE* fp)
{
  if (fp == NULL) { // check if file pointer is valid
    return;
  }
  if (ctrs == NULL) { // handle null counter set
    fprintf(fp, "(null)");
    return;
  }
  fprintf(fp, "{");
  if (ctrs->root != NULL) entry_print(ctrs->root, ctrs->root, fp); // start recursive print
  fprintf(fp, "}\n");
}

/**************** counters_iterate() ****************/
void counters_iterate(counters_t* ctrs, void* arg, void (*itemfunc)(void* arg, const int key, const int count))
{
  if (ctrs == NULL || itemfunc == NULL) { // check input arguments
    return;
  }
  if (ctrs->root != NULL) {
    entry_iterate(ctrs->root, arg, itemfunc); // call helper
  }
}

/**************** counters_new() ****************/
counters_t* counters_new(void)
{
  // exit if errors with allocation
  return (counters_t*) mem_calloc_assert(1, sizeof(counters_t), "Memory for new counters could not be allocated");
}

/**************** counters_add() ****************/
int counters_add(counters_t* ctrs, const int key)
{
  if (ctrs == NULL || key < 0) { // invalid arguments
    fprintf(stderr, "Null counter or key out of range\n");
    return 0;
  }
  counters_entry_t* new_entry = counters_entry_new(key); // create new entry
  if (new_entry == NULL) return 0;

  if (ctrs->root == NULL) { // if tree is empty, set root
    ctrs->root = new_entry;
    return true;
  }
  return add_new_entry(ctrs->root, new_entry); // otherwise insert recursively
}

/**************** counters_get() ****************/
int counters_get(counters_t* ctrs, const int key)
{
  if (ctrs == NULL || key < 0) { // input validation
    fprintf(stderr, "Null counter or key out of range\n");
    return 0;
  }
  if (ctrs->root == NULL) { // if root is null return null
    return 0;
  }
  return counters_get_entry(ctrs->root, key); // else lookup recursively
}

/**************** counters_set() ****************/
bool counters_set(counters_t* ctrs, const int key, const int count)
{
  if (ctrs == NULL || key < 0 || count < 0) { // input validation
    fprintf(stderr, "Null counter or key or count out of range\n");
    return false;
  }
  if (ctrs->root == NULL) { // empty tree case
    ctrs->root = counters_entry_new(key);
    ctrs->root->count = count;
    return true;
  }
  return counters_set_entry(ctrs->root, key, count); // otherwise set recursively
}

/**************** counters_delete() ****************/
void counters_delete(counters_t* ctrs) 
{
  if (ctrs == NULL) {
    fprintf(stderr, "Attempted to delete null counter\n");
    return;
  }
  if (ctrs->root != NULL) {
    counters_entry_delete(ctrs->root); // recursively free entries
    mem_free(ctrs->root);
  }
  mem_free(ctrs); // free main struct
}

/**************** local (static) functions ****************/

/**************** entry_print() ****************/
static void entry_print(counters_entry_t* entry, counters_entry_t* root, FILE* fp)
{
  if (entry != root) fprintf(fp, ", ");
  fprintf(fp, "%d: %d", entry->key, entry->count); // print current entry
  if (hasLeft(entry)) {
    entry_print(entry->left, root, fp);            // recursively call left
  }
  if (hasRight(entry)) {
    entry_print(entry->right, root, fp);           // recursively call right
  }
}

/**************** entry_iterate() ****************/
static void entry_iterate(counters_entry_t* entry, void* arg, void (*itemfunc)(void* arg, const int key, const int count))
{ 
  if (hasLeft(entry)) {
    entry_iterate(entry->left, arg, itemfunc); // recursively call left entry
  }
  itemfunc(arg, entry->key, entry->count);     // apply function to current node
  if (hasRight(entry)) {
    entry_iterate(entry->right, arg, itemfunc); // recursively call right entry
  }
}

/**************** counters_entry_new() ****************/
static counters_entry_t* counters_entry_new(const int key)
{
  counters_entry_t* new_entry = (counters_entry_t*) // assign space for new entry
    mem_calloc_assert(1, sizeof(counters_entry_t), "Memory for new counters entry could not be allocated\n");

  new_entry->key = key;
  new_entry->count = 1;                             // new entries start with count 1

  return new_entry;
}

/**************** counters_set_entry() ****************/
static bool counters_set_entry(counters_entry_t* current_entry, const int key, const int count)
{
  if (current_entry->key == key) {
    current_entry->count = count; // found node
    return true;
  } else if (key < current_entry->key) {
    if (hasLeft(current_entry)) {      // if has left node, recursively call
      return counters_set_entry(current_entry->left, key, count);
    }
    if ((current_entry->left = counters_entry_new(key)) == NULL) return false;
    current_entry->left->count = count;// else insert new node
    return true;
  } else {
    if (hasRight(current_entry)) {     // if has right node, recursively call
      return counters_set_entry(current_entry->right, key, count);
    }
    if ((current_entry->right = counters_entry_new(key)) == NULL) return false;
    current_entry->right->count = count;// else insert new node
    return true;
  }
}

/**************** counters_get_entry() ****************/
static int counters_get_entry(counters_entry_t* current_entry, const int key)
{
  if (current_entry->key == key) {
    return current_entry->count; // found key
  } else if (key < current_entry->key) {
    if (hasLeft(current_entry)) {
      return counters_get_entry(current_entry->left, key);
    }
    return 0; // key not found
  } else {
    if (hasRight(current_entry)) {
      return counters_get_entry(current_entry->right, key);
    }
    return 0; // key not found
  }
}

/**************** add_new_entry() ****************/
static int add_new_entry(counters_entry_t* current_entry, counters_entry_t* entry_to_add)
{
  if (current_entry->key == entry_to_add->key) {
    mem_free(entry_to_add);
    return ++current_entry->count; // increment existing entry
  } else if (current_entry->key > entry_to_add->key) {
    if (hasLeft(current_entry)) {
      return add_new_entry(current_entry->left, entry_to_add); // if has left recursively call
    }
    return (current_entry->left = entry_to_add)->count;        // else insert to left
  } else {
    if (hasRight(current_entry)) {
      return add_new_entry(current_entry->right, entry_to_add);// if has right recursively call
    }
    return (current_entry->right = entry_to_add)->count;       // else insert to right
  }
}

/**************** counters_entry_delete() ****************/
static void counters_entry_delete(counters_entry_t* entry)
{
  if (hasLeft(entry)) {
    counters_entry_delete(entry->left);  // recursively call on left
    mem_free(entry->left);               // free left entry
  }
  if (hasRight(entry)) {
    counters_entry_delete(entry->right); // do the same with the right
    mem_free(entry->right);
  }
}

/**************** hasLeft() ****************/
static bool hasLeft(counters_entry_t* entry)
{
  return (entry->left != NULL); // check left child
}

/**************** hasRight() ****************/
static bool hasRight(counters_entry_t* entry)
{
  return (entry->right != NULL); // check right child
}
