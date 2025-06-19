/*
 * word.c - CS50 'word' module
 *
 * This module implements the normalizeWord function
 *
 * See word.h for more information.
 *
 * Arthur Ufongene, May 2025
 */

#include "word.h"
#include <string.h>
#include <ctype.h>
#include "mem.h"

/********** word_normalizeWord *************/
/* See word.h for more information */
char* word_normalizeWord(const char* word)
{
  // allocate space for normalized string (length of word + null terminator)
  char* normalized = mem_calloc_assert(strlen(word) + 1, sizeof(char),
                              "Couldn't allocate space for normalized word");

  // copy original string to normalized one in lowercase
  for (int i = 0; i < strlen(word); i++) {
    normalized[i] = tolower((unsigned char) word[i]);
  }

  // null terminate
  normalized[strlen(word)] = '\0';
  
  return normalized;
}


/********** word_decomposeSequence ************/
/* see word.h for more information */
char** word_decomposeSequence(char* query)
{
  int arraySize = 100;                
  int place = 0;                      // Index in wordArray
  int length = strlen(query);         
  bool inWord = false;                // Track whether we're currently in a word

  char** wordArray = mem_calloc_assert(arraySize, sizeof(char*), "Couldn't alloc"); // Allocate word pointer array

  for (int i = 0; i < length; i++) {
    // Resize array if we are about to write outside of it
    if (place + 1 >= arraySize) {
      arraySize *= 2;
      wordArray = realloc(wordArray, arraySize * sizeof(char*));
    }

    // If we detect a non-letter non-space character,
    if (!isalpha(query[i]) && !isspace(query[i])) {
      free(wordArray);
    // print error and exit null
      fprintf(stderr, "Error: bad character '%c' in query.\n\n", query[i]);
      return NULL;
    }

    // if we are not in a word yet and detect a letter
    if (!inWord && isalpha(query[i])) {
      wordArray[place] = query + i;   // Set array entry pointer to beginning of word
      place++;
      inWord = true;             
    }

    // If we are in a word and detect a space
    else if (inWord && isspace(query[i])) {
      query[i] = '\0';                // Replace space with null terminator
      inWord = false;
    }
  }

  // Null-terminate remaining array entries
  for (; place < arraySize; place++) {
    wordArray[place] = NULL;
  }

  return wordArray;
}

/********** word_normalizeSequence *************/
/* See word.h for more info */
void word_normalizeSequence(char** sequence)
{
  char* normalizedWord;
  for (int i = 0; sequence[i] != NULL; i++) {
    // normalize each word
    normalizedWord = word_normalizeWord(sequence[i]);

    // overwrite original word with normalized version
    strcpy(sequence[i], normalizedWord);

    // free temporary normalized string
    free(normalizedWord);
  }
}

/********** word_checkSyntax *************/
/* See word.h for more info */
bool word_checkSyntax(char** sequence)
{
  bool operator = false;
  int i = 0;

  for (; sequence[i] != NULL; i++) {
      // if the word in a sequence is an 'and' or 'or'
    if (strcmp(sequence[i], "and") == 0 || strcmp(sequence[i], "or") == 0) {

      // two operators in a row
      if (operator) {
        fprintf(stderr, "Error: '%s' and '%s' cannot be adjacent.\n\n", sequence[i-1], sequence[i]);
        return false;
      }

      // operator is first in sequence
      else if (i == 0) {
        fprintf(stderr, "Error: '%s' cannot be first.\n\n", sequence[i]);
        return false;
      }

      // mark operator flag
      else {
        operator = true;
      }

    } else {
      // current word is not an operator
      operator = false;
    }
  }

      // last word is an operator
  if (operator) {
    fprintf(stderr, "Error: '%s' cannot be last.\n\n", sequence[i-1]);
    return false;
  }
     
     // if no rules broken, return true
  return true;
}


void word_printSequence(char** wordSequence)
{
  printf("Query: ");
  // loop through the sequence printing each word in order
  for (int i = 0; wordSequence[i] != NULL; i++) {
    printf("%s ", wordSequence[i]);
  }
  printf("\n");
}
