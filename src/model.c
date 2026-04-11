#include "embedding.h"
#include "linalg.h"
#include "matrix.h"
#include "tokenizer.h"
#include <assert.h>
#include <stdio.h>

const double LEARNING_RATE = 0.01;
const int EPOCHS = 3;

matrix *forward_pass(matrix *embedding_matrix, matrix *embedding_vector) {
  // return the logits
  matrix *m = matrix_mult(embedding_matrix, embedding_vector);
  m = softmax(m, 1);
  return m;
}

matrix *backprop(matrix *embedding_matrix, matrix *embedding_vector,
                 matrix *forw_pass, int correct_token_index) {

  int vocab_size = embedding_matrix->num_rows;

  // calculating the dL/dz
  matrix *y = one_hot_encoding(correct_token_index, vocab_size);
  matrix *ydiff = matrix_sub(forw_pass, y);

  // dL/dw = dL/dz * dz/dw
  // dL/dz = (ŷ - y)
  // Since z = W * a
  // dz/dw = a
  // Then dL/dw = (ŷ - y) * a
  matrix *emb_t = matrix_transpose(embedding_vector);
  matrix *dl_dz = matrix_mult(ydiff, emb_t);
  matrix *grad_desc = matrix_scalar_mult(dl_dz, LEARNING_RATE);

  // Updating the embedding matrix
  matrix *new_embedding_matrix = matrix_sub(embedding_matrix, grad_desc);

  // Freeing some memory
  matrix_free(y);
  matrix_free(ydiff);
  matrix_free(emb_t);
  matrix_free(grad_desc);
  matrix_free(embedding_matrix);
  matrix_free(dl_dz);

  return new_embedding_matrix;
}

matrix *train(Node **token_map, matrix *embedding_matrix, char *fpath,
              int window_size) {

  assert(window_size > 1);

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

  for (int epoch = 1; epoch < EPOCHS + 1; epoch++) {

    // loss per epoch
    double loss_sum = 0;

    // Initialize the window array
    if (epoch > 1)
      free(window->data);
    char **arr_data = (char **)malloc(window_size * sizeof(char *));
    window->data = arr_data;
    window->count = 0;
    window->size = window_size;

    for (size_t i = 0; i < (tokens->count - 1); i++) {
      if (i % 100 == 0 && i > 0) {
        avg_loss = loss_sum / i;
        printf("[Epoch %d / %d] Running %d out of %d / Loss = %.5f\n", epoch,
               EPOCHS, (int)i, (int)tokens->count, avg_loss);
      }

      if (avg_loss > 0 && avg_loss < 1e-1) {
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
      loss_sum += cross_entropy_loss(logits, target);

      embedding_matrix = backprop(embedding_matrix, emb, logits, target);

      matrix_free(emb);
      matrix_free(logits);

      window = pop_from_array(window);
    }
  }

  fclose(fptr);

  return embedding_matrix;
}
