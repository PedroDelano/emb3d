#include "embedding.h"
#include "model.h"
#include "tokenizer.h"
#include <stdio.h>

int main(void) {

  char *VOCAB_FILE = "data/vocab_small.txt";
  char *TRAIN_FILE = "data/train.txt";
  const int WINDOW_SIZE = 3;

  Node **token_map = generate_token_map(VOCAB_FILE);
  matrix *embedding_matrix = embedding_matrix_new(get_vocab_size(VOCAB_FILE));

  embedding_matrix =
      train(token_map, embedding_matrix, TRAIN_FILE, WINDOW_SIZE);

  return 0;
}
