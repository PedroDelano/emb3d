#include "embedding.h"
#include "tokenizer.h"
#include <stdio.h>

int main(void) {

  char *VOCAB_PATH = "data/vocab.txt";
  Node **token_map = generate_token_map(VOCAB_PATH);
  matrix *embedding_matrix = embedding_matrix_new(get_vocab_size(VOCAB_PATH));

  char *query = "the good, the bad, the ugly.";
  double *vec = embedding(embedding_matrix, token_map, query);

  for (int i = 0; i < (int)EMBEDDING_SIZE; i++) {
    printf("%.2f / ", vec[i]);
  }

  return 0;
}
