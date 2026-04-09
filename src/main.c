#include "hashmap.h"
#include "linalg.h"
#include "linkedlist.h"
#include "matrix.h"
#include "tokenizer.h"
#include <stdio.h>

int main(void) {

  Node **hashmap = generate_token_map("data/vocab.txt");

  char *query = "bug";
  int enc = tk_encode(hashmap, query);
  char *dec = tk_decode(hashmap, enc);
  printf("The idx of %s is %d ----- \n", dec, enc);

  query = "hello";
  enc = tk_encode(hashmap, query);
  dec = tk_decode(hashmap, enc);
  printf("The idx of %s is %d ----- \n", dec, enc);

  Array *tokens = tokenize("The good, the bad and the ugly.");
  for (size_t i = 0; i < tokens->size; i++) {
    printf("%s\n", tokens->data[i]);
  }

  return 0;
}
