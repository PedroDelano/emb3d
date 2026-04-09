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

  query = "meaning";
  enc = tk_encode(hashmap, query);
  dec = tk_decode(hashmap, enc);
  printf("The idx of %s is %d ----- \n", dec, enc);

  return 0;
}
