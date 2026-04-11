#include "embedding.h"
#include "linalg.h"
#include "model.h"
#include "tokenizer.h"
#include <stdio.h>

int main(void) {

  char *VOCAB_FILE = "data/vocab_small.txt";
  char *TRAIN_FILE = "data/train_small.txt";
  const int WINDOW_SIZE = 3;

  Node **token_map = generate_token_map(VOCAB_FILE);
  matrix *embedding_matrix = embedding_matrix_new(get_vocab_size(VOCAB_FILE));

  embedding_matrix =
      train(token_map, embedding_matrix, TRAIN_FILE, WINDOW_SIZE);

  // Similar pairs
  char *similar[][2] = {
      {"machine", "learning"},    {"sentiment", "analysis"},
      {"neural", "network"},      {"good", "excellent"},
      {"data", "datasets"},       {"supervised", "unsupervised"},
      {"statistical", "methods"}, {"text", "language"},
  };

  // Dissimilar pairs
  char *dissimilar[][2] = {
      {"hotel", "neural"},      {"food", "parsing"},
      {"anger", "algorithms"},  {"machine", "food"},
      {"hotel", "statistical"}, {"anger", "data"},
      {"praise", "computing"},  {"disgust", "optimization"},
  };

  int num_similar = 8;
  int num_dissimilar = 8;

  printf("\n=== Similar Pairs ===\n");
  for (int i = 0; i < num_similar; i++) {
    int id_a = tk_encode(token_map, similar[i][0]);
    int id_b = tk_encode(token_map, similar[i][1]);
    matrix *vec_a = matrix_to_col_vec(
        embedding_retrieve(embedding_matrix, id_a), EMBEDDING_SIZE);
    matrix *vec_b = matrix_to_col_vec(
        embedding_retrieve(embedding_matrix, id_b), EMBEDDING_SIZE);
    double sim = cosine_similarity(vec_a, vec_b);
    printf("  %-15s vs %-15s = %.4f\n", similar[i][0], similar[i][1], sim);
    matrix_free(vec_a);
    matrix_free(vec_b);
  }

  printf("\n=== Dissimilar Pairs ===\n");
  for (int i = 0; i < num_dissimilar; i++) {
    int id_a = tk_encode(token_map, dissimilar[i][0]);
    int id_b = tk_encode(token_map, dissimilar[i][1]);
    matrix *vec_a = matrix_to_col_vec(
        embedding_retrieve(embedding_matrix, id_a), EMBEDDING_SIZE);
    matrix *vec_b = matrix_to_col_vec(
        embedding_retrieve(embedding_matrix, id_b), EMBEDDING_SIZE);
    double sim = cosine_similarity(vec_a, vec_b);
    printf("  %-15s vs %-15s = %.4f\n", dissimilar[i][0], dissimilar[i][1],
           sim);
    matrix_free(vec_a);
    matrix_free(vec_b);
  }

  return 0;
}
