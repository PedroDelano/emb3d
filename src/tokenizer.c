/*
 * A tokenizer must:
 * - Given a list of tokens (i.e. vocab) of return a map that links a word with
 * an integer;
 * - Must have encode/decode functions for a list and a single value
 * - It should be able to tokenize (i.e. break raw text into pieces and map into
 * an integer - encode)
 * */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// A token can only have 256 letters
const int MAX_TOKEN_SIZE = 255;

/*********************************************
 * TOKENIZER IMPLEMENTATION
*********************************************/

char **generate_token_map(char *fpath) {
  // this function will return an array
  // that maps an integer to (a pointer of) a string
  FILE *fptr;
  char buffer[MAX_TOKEN_SIZE];

  fptr = fopen(fpath, "r");
  assert(fptr != NULL && "Could not open file.");

  int n_tokens = 0;
  while (fgets(buffer, 255, fptr)) {
    n_tokens++;
  }

  assert(n_tokens > 0 && "empty vocab file");

  // allocated an array of size n_tokens
  char **map = malloc(n_tokens * sizeof(char *));

  // going back to the first line
  rewind(fptr);

  int idx = 0;
  while (fgets(buffer, 255, fptr)) {

    // allocating memory for the string
    size_t len = strlen(buffer) + 1;
    char *dest = (char *)malloc(len);

    // copying the string into memory
    strcpy(dest, buffer);

    // filling the array with the pointer to the string
    map[idx] = dest;

    idx++;
  }

  fclose(fptr);

  return map;
}
