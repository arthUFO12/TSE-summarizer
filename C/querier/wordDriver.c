#include <stdlib.h>
#include "file.h"
#include "word.h"


int main() {
  char* query;
  char** wordSequence;
  while ((query = file_readLine(stdin)) != NULL) {
    
    printf("%s\n", query);

    if ((wordSequence = word_decomposeSequence(query)) == NULL) {
      free(query);
      puts("false\n");
      continue;
    }
    // If query is empty, do nothing
    if (wordSequence[0] == NULL) {
      free(query);
      free(wordSequence);
      puts("Empty query: false\n");
      continue;
    }

    // Normalize the sequence of words and then check the syntax
    word_normalizeSequence(wordSequence);
    if (!word_checkSyntax(wordSequence)) {
      free(wordSequence);
      free(query);

      // prompt user again if incorrect syntax
      puts("false\n");
      continue;
    }
    puts("true\n");
  }
  exit(0);
}