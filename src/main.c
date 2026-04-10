#include "embedding.h"
#include "model.h"
#include "tokenizer.h"
#include <stdio.h>

int main(void) {

  char *VOCAB_PATH = "data/vocab.txt";
  Node **token_map = generate_token_map(VOCAB_PATH);
  matrix *embedding_matrix = embedding_matrix_new(get_vocab_size(VOCAB_PATH));

  char *query = "the good, the bad, the ugly.";
  matrix *vec = embedding(embedding_matrix, token_map, query);

  matrix *fp = forward_pass(embedding_matrix, vec);
  matrix_print(fp);

  return 0;
}
