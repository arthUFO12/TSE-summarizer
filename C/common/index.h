/* 
 * index.h - header file for CS50 'index' data structure module
 * 
 * An *index* stores a set of words each of which have a set of id-count pairs
 * A word can only occur in the index once, and an id can only occur once
 * in each word's set. Functions are provided to save the index to a file.
 * An index can either be built from scratch, or reconstructed from a save file.
 * 
 * This data structure is a wrapper for the hashtable, with each of its items
 * being a pointer to the counters data structure. 
 *
 * Arthur Ufongene, May 2025
 */

#include <stdbool.h>
#include "counters.h"
/********* Global Type ***********/
typedef struct index index_t;

/********** Functions ***********/

/*********** index_new ***********/
/* Creates an empty index
 * 
 * Caller provides:
 *   An integer size that is > 0
 * We return:
 *   A valid pointer to an index data structure,
 *   or NULL if invalid size or problems with 
 *   allocation
 * Caller is responsible for:
 *   Later calling index_delete
 */
index_t* index_new(int size);

/*********** index_incrementCount ***********/
/* Increments the count of an ID for a specific word by 1
 * 
 * Caller provides:
 *   A valid pointer to an index, a null terminated string,
 *   and a positive integer ID
 * We return:
 *   An integer denoting the current value of the ID for that word,
 *   if word or idx is NULL, or if error, we return 0
 * We guarantee:
 *   The count for the ID and word given will be incremented by 1
 * Notes:
 *   If an entry for a word doesn't exist, it will be created.
 *   The data structure copies the string to store, so the caller
 *   does not need to worry about storing the string themselves.
 */
int index_incrementCount(index_t* idx, const char* word, int id);

/*********** index_incrementCount ***********/
/* Deletes an index, returning all memory to operating system
 * 
 * Caller provides:
 *   A valid pointer to an index
 * We guarantee:
 *   All memory relating to the index will be freed
 * We do:
 *   If index == NULL, nothing else
 *   Delete the index
 */
void index_delete(index_t* idx);

/********** index_save *************/
/* Saves an index to a specified file using the syntax
 * from the requirements.
 * 
 * Caller provides:
 *   A valid pointer to an index and the name of the file to be saved to
 * We guarantee:
 *   The entirety of the index's data will be saved to the file
 *   in the correct format
 * We do:
 *   If index == NULL or file can't be opened, return false; else
 *   write the contents of the index to the file with each
 *   word entry on a new line and return true;
 */
bool index_save(index_t* idx, char* filename);

/************* index_insertCount *************/
/* Inserts a count for the specified word and ID
 * 
 * Caller provides:
 *   A valid pointer to an index, a null terminated string,
 *   a positive integer ID, and a non-negative integer count
 * We return:
 *   A boolean denoting successful insertion of the count into the index,
 *   will be false if idx or word is NULL or if error
 * Notes:
 *   If an entry for a word doesn't exist, it will be created.
 *   The data structure copies the string to store, so the caller
 *   does not need to worry about storing the string themselves.
 */
bool index_insertCount(index_t* idx, const char* word, int id, int count);

/********** index_reconstruct *************/
/* Reconstructs an index from a saved index file
 * 
 * Caller provides:
 *   The name of a file containing saved index data
 * We return:
 *   A pointer to the in memory index if the file 
 *   can be read, NULL if otherwise
 * Note:
 *   We assume that the lines of the file are in the format:
 *   word id count [id count ...]
 */
index_t* index_reconstruct(char* oldFilename);


/********** index_get *************/
/* Returns a pointer to the counter for a word
 * 
 * Caller provides:
 *   An index and a word to retrieve the counter for
 * We return:
 *   A pointer to the counter or NULL if word isn't in
 *   index
 */
counters_t* index_get(index_t* idx, char* word);
