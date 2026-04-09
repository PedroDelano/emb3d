#include "hashmap.h"
#include "linalg.h"
#include "linkedlist.h"
#include "matrix.h"
#include "tokenizer.h"
#include <stdio.h>

int main(void) {

  Node **hashmap = generate_token_map("data/vocab.txt");

  Array *tokens = tokenize("The good, the bad and the ugly. Spare me none!");
  for (size_t i = 0; i < tokens->size; i++) {
    int enc = tk_encode(hashmap, tokens->data[i]);
    char *dec = tk_decode(hashmap, enc);
    printf("%s --> %d --> %s\n", tokens->data[i], enc, dec);
  }

  return 0;
}
