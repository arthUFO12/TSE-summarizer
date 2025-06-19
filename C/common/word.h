/* 
 * word.h - header file for CS50 'word' module
 * 
 * This module provides a function to normalize words
 *
 * Arthur Ufongene, May 2025
 */

#include <stdbool.h>

/********** word_normalizeWord *************/
/* Returns a string of the normalized word
 *
 * Caller provides:
 *   Valid, null-terminated string
 * We return:
 *   A normalized (converted to lowercase) version of the string
 * Caller is responsible for:
 *   Later calling free on the returned string
 */
char* word_normalizeWord(const char* word);

/********** word_normalizeSequence *************/
/* Normalizes each word in a NULL-terminated string array
 *
 * Caller provides:
 *   A NULL-terminated array of strings
 * We modify:
 *   Each string in the array is converted to lowercase
 * Notes:
 *   Overwrites each original string with its normalized form
 */
void word_normalizeSequence(char** sequence);

/********** word_checkSyntax *************/
/* Validates the syntax of a word sequence
 *
 * Caller provides:
 *   A NULL-terminated array of strings
 * We return:
 *   true if sequence has valid operator syntax, false otherwise
 * Details:
 *   - "and"/"or" cannot be first or last
 *   - "and"/"or" cannot be adjacent
 *   Prints error messages to stderr on failure
 */
bool word_checkSyntax(char** sequence);


/********** word_decomposeSequence *************/
/* Tokenize a query string into individual words
 *
 * This function splits a single string into an array of words.
 * All non-alphabetic, non-space characters are considered invalid.
 *
 * Caller provides:
 *   A null-terminated query string (char*) which may be modified by this function.
 * We return:
 *   A NULL-terminated array of pointers to the beginning of each word in `query`.
 *   If the query contains invalid characters, returns NULL 
 * Caller is responsible for:
 *   Freeing the returned array of words
 * 
 * Notes:
 *   Modifies the input string by inserting null terminators (`\0`) after each word.
 */
char** word_decomposeSequence(char* query);

/************* word_printSequence ****************/
/* Prints a sequence of words to stdout
 *
 * Caller provides: a valid pointer to an array of strings terminated by a NULL
 * We do: Print all words on the same line separated by spaces
 */
void word_printSequence(char** wordSequence);
