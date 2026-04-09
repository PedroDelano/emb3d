#include "embedding.h"
#include "hashmap.h"
#include "linalg.h"
#include "linkedlist.h"
#include "matrix.h"
#include "tokenizer.h"
#include <stdio.h>

int main(void) {

  char *VOCAB_PATH = "data/vocab.txt";
  Node **hashmap = generate_token_map(VOCAB_PATH);

  Array *tokens = tokenize("The good, the bad and the ugly. Spare me none!");
  for (size_t i = 0; i < tokens->size; i++) {
    int enc = tk_encode(hashmap, tokens->data[i]);
    char *dec = tk_decode(hashmap, enc);
    printf("%s --> %d --> %s\n", tokens->data[i], enc, dec);
  }

  matrix *emb = embedding_matrix_new(get_vocab_size(VOCAB_PATH));
  matrix_print(emb);

  return 0;
}
