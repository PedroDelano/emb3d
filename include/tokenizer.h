#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "hashmap.h"
#include <stdlib.h>

typedef struct Array {
  char **data;
  size_t count;
  size_t size;
} Array;

Node **generate_token_map(char *fpath);
int tk_encode(Node **token_map, char *value);
char *tk_decode(Node **token_map, int token_id);
Array *tokenize(char *str);
unsigned int get_vocab_size(char *fpath);

#endif
