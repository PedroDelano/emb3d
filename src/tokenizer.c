/*
 * A tokenizer must:
 * - Given a list of tokens (i.e. vocab) of return a map that links a word with
 * an integer;
 * - Must have encode/decode functions for a list and a single value
 * - It should be able to tokenize (i.e. break raw text into pieces and map into
 * an integer - encode)
 * */

#include "hashmap.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// A token can only have 256 letters
const int MAX_TOKEN_SIZE = 255;
const char *UNKOWN_TOKEN = "<unk>";

/*********************************************
 * TOKENIZER IMPLEMENTATION
 *********************************************/

Node **generate_token_map(char *fpath) {
  // this function will return an array
  // that maps an integer to (a pointer of) a string
  FILE *fptr;
  char buffer[MAX_TOKEN_SIZE];

  // Create the hashmap
  Node **hashmap = hashmap_start();

  fptr = fopen(fpath, "r");
  assert(fptr != NULL && "Could not open file.");

  int n_tokens = 0;
  while (fgets(buffer, 255, fptr)) {

    // Size of buffer without the trailing
    // \n at the end
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
      buffer[len - 1] = '\0';
    }

    // allocate the string in memory
    char *dest = malloc(len);
    strcpy(dest, buffer);

    // insert string in hashmap
    hashmap_insert(hashmap, dest, n_tokens);
    n_tokens++;
  }

  assert(n_tokens > 0 && "empty vocab file");

  return hashmap;
}

int tk_encode(Node **token_map, char *value) {
  return hashmap_retrieve(token_map, value);
}

char *tk_decode(Node **token_map, int token_id) {
  // Linear search through hashmap structure
  for (int i = 0; i < SLOT_SIZE; i++) {
    char *s = search_ll_by_index(token_map[i], token_id);
    if (strcmp(s, UNKOWN_TOKEN) != 0) {
      return s;
    }
  }
  return (char *)UNKOWN_TOKEN;
}
