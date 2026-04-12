#include "embedding.h"
#include "linalg.h"
#include "model.h"
#include "tokenizer.h"
#include <stdio.h>
#include <string.h>

void print_usage(char *prog) {
  printf("Usage:\n");
  printf("  %s train <vocab> <data> <window_size> <emb_size> <lr> <epochs> <output_model>\n", prog);
  printf("  %s eval <vocab> <model>\n", prog);
  printf("  %s similar <vocab> <model> <word_a> <word_b>\n", prog);
}

void run_eval(Node **token_map, matrix *embedding_matrix) {
  char *similar[][2] = {
      {"software", "hardware"},   {"algorithm", "data"},
      {"network", "internet"},    {"server", "database"},
      {"linux", "kernel"},        {"encryption", "security"},
      {"processor", "memory"},    {"code", "programming"},
  };

  char *dissimilar[][2] = {
      {"software", "river"},      {"algorithm", "painting"},
      {"network", "forest"},      {"server", "mountain"},
      {"linux", "ocean"},         {"encryption", "garden"},
      {"processor", "novel"},     {"code", "weather"},
  };

  int num_similar = 8;
  int num_dissimilar = 8;

  double avg_similar = 0.0;
  printf("\n=== Similar Pairs ===\n");
  for (int i = 0; i < num_similar; i++) {
    int id_a = tk_encode(token_map, similar[i][0]);
    int id_b = tk_encode(token_map, similar[i][1]);
    matrix *vec_a = matrix_to_col_vec(
        embedding_retrieve(embedding_matrix, id_a), EMBEDDING_SIZE);
    matrix *vec_b = matrix_to_col_vec(
        embedding_retrieve(embedding_matrix, id_b), EMBEDDING_SIZE);
    double sim = cosine_similarity(vec_a, vec_b);
    avg_similar += sim;
    printf("  %-15s vs %-15s = %.4f\n", similar[i][0], similar[i][1], sim);
    matrix_free(vec_a);
    matrix_free(vec_b);
  }
  avg_similar /= num_similar;
  printf("  Average: %.4f\n", avg_similar);

  double avg_dissimilar = 0.0;
  printf("\n=== Dissimilar Pairs ===\n");
  for (int i = 0; i < num_dissimilar; i++) {
    int id_a = tk_encode(token_map, dissimilar[i][0]);
    int id_b = tk_encode(token_map, dissimilar[i][1]);
    matrix *vec_a = matrix_to_col_vec(
        embedding_retrieve(embedding_matrix, id_a), EMBEDDING_SIZE);
    matrix *vec_b = matrix_to_col_vec(
        embedding_retrieve(embedding_matrix, id_b), EMBEDDING_SIZE);
    double sim = cosine_similarity(vec_a, vec_b);
    avg_dissimilar += sim;
    printf("  %-15s vs %-15s = %.4f\n", dissimilar[i][0], dissimilar[i][1],
           sim);
    matrix_free(vec_a);
    matrix_free(vec_b);
  }
  avg_dissimilar /= num_dissimilar;
  printf("  Average: %.4f\n", avg_dissimilar);
}

int main(int argc, char *argv[]) {

  if (argc < 2) {
    print_usage(argv[0]);
    return 1;
  }

  char *cmd = argv[1];

  if (strcmp(cmd, "train") == 0) {
    if (argc < 9) {
      printf("Usage: %s train <vocab> <data> <window_size> <emb_size> <lr> <epochs> <output_model>\n", argv[0]);
      return 1;
    }
    char *vocab = argv[2];
    char *data = argv[3];
    int window_size = atoi(argv[4]);
    int emb_size = atoi(argv[5]);
    LEARNING_RATE = atof(argv[6]);
    EPOCHS = atoi(argv[7]);
    char *output = argv[8];

    embedding_set_size(emb_size);
    Node **token_map = generate_token_map(vocab);
    matrix *embedding_matrix = embedding_matrix_new(get_vocab_size(vocab));

    embedding_matrix = train(token_map, embedding_matrix, data, window_size);

    if (matrix_save(embedding_matrix, output) == 0) {
      printf("Model saved to %s\n", output);
    } else {
      printf("Error saving model\n");
      return 1;
    }

  } else if (strcmp(cmd, "eval") == 0) {
    if (argc < 4) {
      printf("Usage: %s eval <vocab> <model>\n", argv[0]);
      return 1;
    }
    char *vocab = argv[2];
    char *model_path = argv[3];

    Node **token_map = generate_token_map(vocab);
    matrix *embedding_matrix = matrix_load(model_path);
    if (embedding_matrix == NULL) {
      printf("Error loading model from %s\n", model_path);
      return 1;
    }
    embedding_set_size(embedding_matrix->num_cols);

    run_eval(token_map, embedding_matrix);

  } else if (strcmp(cmd, "similar") == 0) {
    if (argc < 6) {
      printf("Usage: %s similar <vocab> <model> <word_a> <word_b>\n", argv[0]);
      return 1;
    }
    char *vocab = argv[2];
    char *model_path = argv[3];
    char *word_a = argv[4];
    char *word_b = argv[5];

    Node **token_map = generate_token_map(vocab);
    matrix *embedding_matrix = matrix_load(model_path);
    if (embedding_matrix == NULL) {
      printf("Error loading model from %s\n", model_path);
      return 1;
    }
    embedding_set_size(embedding_matrix->num_cols);

    int id_a = tk_encode(token_map, word_a);
    int id_b = tk_encode(token_map, word_b);
    matrix *vec_a = matrix_to_col_vec(
        embedding_retrieve(embedding_matrix, id_a), EMBEDDING_SIZE);
    matrix *vec_b = matrix_to_col_vec(
        embedding_retrieve(embedding_matrix, id_b), EMBEDDING_SIZE);
    double sim = cosine_similarity(vec_a, vec_b);
    printf("%.4f\n", sim);
    matrix_free(vec_a);
    matrix_free(vec_b);

  } else {
    print_usage(argv[0]);
    return 1;
  }

  return 0;
}
