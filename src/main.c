#include "hashmap.h"
#include "linalg.h"
#include "linkedlist.h"
#include "matrix.h"
#include "tokenizer.h"
#include <stdio.h>

int main(void) {

  Node **hashmap = generate_token_map("data/vocab.txt");
  char *query = "bug";
  int idx = hashmap_retrieve(hashmap, query);
  printf("The idx of %s is %d ----- \n", query, idx);

  return 0;
}
