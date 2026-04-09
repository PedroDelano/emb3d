#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "hashmap.h"

Node **generate_token_map(char *fpath);
int tk_encode(Node **token_map, char *value);
char *tk_decode(Node **token_map, int token_id);

#endif
