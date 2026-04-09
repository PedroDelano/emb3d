/*
 * A tokenizer must:
 * - Given a list of tokens (i.e. vocab) of return a map that links a word with
 * an integer;
 * - Must have encode/decode functions for a list and a single value
 * - It should be able to tokenize (i.e. break raw text into pieces and map into
 * an integer - encode)
 * */

#include "tokenizer.h"
#include "hashmap.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// A token can only have 256 letters
const int MAX_TOKEN_SIZE = 255;
const char *UNKOWN_TOKEN = "<unk>";

/*********************************************
 * TOKENIZER MAP
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

/*********************************************
 * TOKENIZER ENCODERS AND DECODERS
 *********************************************/

int tk_encode(Node **token_map, char *value) {
  return hashmap_retrieve(token_map, value);
}

char *tk_decode(Node **token_map, int token_id) {
  // Linear search through hashmap structure
  for (int i = 0; i < SLOT_SIZE; i++) {
    char *s = search_ll_by_index(token_map[i], token_id);
    if (s != NULL) {
      return s;
    }
  }
  return (char *)UNKOWN_TOKEN;
}

Array *tokenize(char *str) {
  size_t str_size = strlen(str);

  // We need to allocate the empty word to be able to
  // change it (append chars to it)
  char *_word = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
  strcpy(_word, "");

  // Here we allocate our dynamic array
  int arr_size = 2;
  char **arr_data = (char **)malloc(arr_size * sizeof(char *));

  Array *arr = (Array *)malloc(sizeof(Array));
  arr->data = arr_data;
  arr->count = 0;
  arr->size = arr_size;

  for (size_t i = 0; i < str_size; i++) {

    if (str[i] == ' ') {

      // the word reached the end
      // we need to append the word
      // to the array
      if (arr->count == arr->size) {
        // increase size
        arr->size = arr->size * 2;
        arr->data = (char **)realloc(arr->data, arr->size * sizeof(char *));
      }

      // allocate the memory for the word
      // so that we can change the value of
      // _word without changing the contents
      // of arr.data
      size_t word_len = strlen(_word) + 1;
      char *w = (char *)malloc(word_len);
      strcpy(w, _word);

      arr->data[arr->count] = w;
      arr->count++;
      strcpy(_word, "");

      continue;
    }

    // append character
    _word = strncat(_word, &str[i], 1);
  }

  if (strlen(_word) > 0) {
    if (arr->count == arr->size) {
      arr->size = arr->size * 2;
      arr->data = (char **)realloc(arr->data, arr->size * sizeof(char *));
    }

    size_t word_len = strlen(_word) + 1;
    char *w = (char *)malloc(word_len);
    strcpy(w, _word);

    arr->data[arr->count] = w;
    arr->count++;
    strcpy(_word, "");
  }

  // we are done with _word so we better
  // free the memory
  free(_word);

  // set the actual final size
  arr->size = arr->count;

  return arr;
}
