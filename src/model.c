#include "embedding.h"
#include "linalg.h"
#include "matrix.h"
#include "tokenizer.h"
#include "learning_rate.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

double LEARNING_RATE = 0.01;
int EPOCHS = 3;

void eval_pairs(Node **token_map, matrix *embedding_matrix) {
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

  double sim_vals[8], dis_vals[8];

  double avg_similar = 0.0;
  for (int i = 0; i < num_similar; i++) {
    int id_a = tk_encode(token_map, similar[i][0]);
    int id_b = tk_encode(token_map, similar[i][1]);
    matrix *vec_a = matrix_to_col_vec(
        embedding_retrieve(embedding_matrix, id_a), EMBEDDING_SIZE);
    matrix *vec_b = matrix_to_col_vec(
        embedding_retrieve(embedding_matrix, id_b), EMBEDDING_SIZE);
    sim_vals[i] = cosine_similarity(vec_a, vec_b);
    avg_similar += sim_vals[i];
    matrix_free(vec_a);
    matrix_free(vec_b);
  }
  avg_similar /= num_similar;
  double std_similar = 0.0;
  for (int i = 0; i < num_similar; i++)
    std_similar += (sim_vals[i] - avg_similar) * (sim_vals[i] - avg_similar);
  std_similar = sqrt(std_similar / num_similar);
  printf("\tSimilar pairs: %.4f ± %.4f\n", avg_similar, std_similar);

  double avg_dissimilar = 0.0;
  for (int i = 0; i < num_dissimilar; i++) {
    int id_a = tk_encode(token_map, dissimilar[i][0]);
    int id_b = tk_encode(token_map, dissimilar[i][1]);
    matrix *vec_a = matrix_to_col_vec(
        embedding_retrieve(embedding_matrix, id_a), EMBEDDING_SIZE);
    matrix *vec_b = matrix_to_col_vec(
        embedding_retrieve(embedding_matrix, id_b), EMBEDDING_SIZE);
    dis_vals[i] = cosine_similarity(vec_a, vec_b);
    avg_dissimilar += dis_vals[i];
    matrix_free(vec_a);
    matrix_free(vec_b);
  }
  avg_dissimilar /= num_dissimilar;
  double std_dissimilar = 0.0;
  for (int i = 0; i < num_dissimilar; i++)
    std_dissimilar += (dis_vals[i] - avg_dissimilar) * (dis_vals[i] - avg_dissimilar);
  std_dissimilar = sqrt(std_dissimilar / num_dissimilar);
  printf("\tDissimilar pairs: %.4f ± %.4f\n", avg_dissimilar, std_dissimilar);

  double sigma = sqrt(std_similar * std_similar + std_dissimilar * std_dissimilar);
  double separation = avg_similar - avg_dissimilar;
  double compat = sigma > 0 ? separation / sigma : 0;
  printf("\tCompatibility: %.4f (sep=%.4f, σ=%.4f)\n", compat, separation, sigma);
}

matrix *forward_pass(matrix *embedding_matrix, matrix *embedding_vector) {
  // return the logits
  matrix *m = matrix_mult(embedding_matrix, embedding_vector);
  m = softmax(m, 1);
  return m;
}

matrix *backprop(matrix *embedding_matrix, matrix *embedding_vector,
                 matrix *forw_pass, int correct_token_index, double learning_rate) {

  int vocab_size = embedding_matrix->num_rows;

  // calculating the dL/dz = (ŷ - y)
  matrix *y = one_hot_encoding(correct_token_index, vocab_size);
  matrix_sub_r(forw_pass, y);

  // dL/dw = dL/dz * dz/dw = (ŷ - y) * a^T
  matrix *emb_t = matrix_transpose(embedding_vector);
  matrix *grad = matrix_mult(forw_pass, emb_t);
  matrix_scalar_mult_r(grad, learning_rate);

  // W = W - lr * gradient
  matrix_sub_r(embedding_matrix, grad);

  // Freeing some memory
  matrix_free(y);
  matrix_free(emb_t);
  matrix_free(grad);

  return embedding_matrix;
}

matrix *train(Node **token_map, matrix *embedding_matrix, char *fpath,
              int window_size) {

  assert(window_size > 0);

  printf("[DEBUG] train() started\n");

  FILE *fptr;
  fptr = fopen(fpath, "r");
  printf("[DEBUG] file opened\n");

  fseek(fptr, 0, SEEK_END);
  long size = ftell(fptr) + 1;
  fseek(fptr, 0, SEEK_SET);
  printf("[DEBUG] file size: %ld bytes\n", size);

  char *file_content = malloc(size * sizeof(char));
  fread(file_content, sizeof(char), size, fptr);
  file_content[size - 1] = '\0';
  printf("[DEBUG] file read into memory\n");

  Array *tokens = tokenize(file_content);
  free(file_content);
  printf("[DEBUG] tokenized: %d tokens\n", (int)tokens->count);

  // Define window_size array
  Array *window = (Array *)malloc(sizeof(Array));

  int target = -1;
  double avg_loss = 0;
  double lr = LEARNING_RATE;

  for (int epoch = 1; epoch < EPOCHS + 1; epoch++) {

    // loss per epoch
    double loss_sum = 0;
    double last_avg_loss = 0;
    double loss_diff = 0;

    // token-miss
    int token_miss = 0;
    double token_miss_rate = 0;

    // Initialize the window array
    if (epoch > 1)
      free(window->data);
    char **arr_data = (char **)malloc(window_size * sizeof(char *));
    window->data = arr_data;
    window->count = 0;
    window->size = window_size;

    int unk = tk_encode(token_map, (char*)UNKOWN_TOKEN);

    for (size_t i = 0; i < (tokens->count - 1); i++) {
      if (i % 5000 == 0 && i > 0) {
        token_miss_rate = 100.0 * token_miss / i;
        avg_loss = loss_sum / i;
        loss_diff = last_avg_loss > 0 ? 100.0 * (avg_loss - last_avg_loss) / last_avg_loss : 0;
        printf("[Epoch %d / %d] Running %d out of %d / Loss = %.5f (%.3f %%) / Tk. Miss = %.1f %% \n", epoch,
               EPOCHS, (int)i, (int)tokens->count, avg_loss, loss_diff, token_miss_rate);
        last_avg_loss = avg_loss;
        eval_pairs(token_map, embedding_matrix);
        printf("\tCurrent LR = %.5f\n", lr);
      }

      if (avg_loss > 0 && avg_loss < 1e-3) {
        printf("EARLY STOP: Stopped at average loss = %.5f\n", avg_loss);
        return embedding_matrix;
      }

      window = add_to_array(window, tokens->data[i]);

      // Filling the array
      if (i < (size_t)window_size - 1)
        continue;

      matrix *emb = embedding(embedding_matrix, token_map, window);
      matrix *logits = forward_pass(embedding_matrix, emb);

      target = tk_encode(token_map, tokens->data[i + 1]);

      if (target == unk) {
        token_miss++;
        matrix_free(emb);
        matrix_free(logits);
        window = pop_from_array(window);
        continue;
      }

      loss_sum += cross_entropy_loss(logits, target);
      lr = lr_cosine(LEARNING_RATE, i, tokens->count);
      embedding_matrix = backprop(embedding_matrix, emb, logits, target, lr);

      matrix_free(emb);
      matrix_free(logits);

      window = pop_from_array(window);
    }

  }

  fclose(fptr);

  return embedding_matrix;
}
