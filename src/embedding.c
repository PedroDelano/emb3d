/*********************************
 *
 * Simple embedding implementation
 *
 *********************************/

#include "embedding.h"
#include "tokenizer.h"
#include <assert.h>
#include <math.h>

unsigned int EMBEDDING_SIZE = 32;

void embedding_set_size(unsigned int size) { EMBEDDING_SIZE = size; }

double sparcity_score(matrix *embedding_matrix, double epsilon) {
  unsigned int total = embedding_matrix->num_rows * embedding_matrix->num_cols;
  unsigned int count = 0;
  for (unsigned int i = 0; i < embedding_matrix->num_rows; i++) {
    for (unsigned int j = 0; j < embedding_matrix->num_cols; j++) {
      if (fabs(embedding_matrix->data[i][j]) < epsilon) {
        count++;
      }
    }
  }
  return (double)count / (double)total;
}

matrix *embedding_matrix_new(unsigned int vocab_size) {
  return matrix_new_orthogonal(vocab_size, EMBEDDING_SIZE);
}

double *embedding_retrieve(matrix *embedding_matrix, int token_id) {
  assert(token_id < (int)embedding_matrix->num_rows &&
         "ERROR: invalid token_id (greater than vocab_size)");
  return embedding_matrix->data[token_id];
}

matrix *one_hot_encoding(int index, int size) {
  // Returns a [0, 0, 1, ..., 0] vector
  // where x[index] = 1
  matrix *m = matrix_new(size, 1);
  m->data[index][0] = 1;
  return m;
}

matrix *embedding(matrix *embedding_matrix, Node **token_map, Array *tokens) {

  /*
   * Simple embedding implementation via averaging
   * embedding vectors
   * */

  // allocating memory for a vector of doubles
  double *emb_vec = malloc(EMBEDDING_SIZE * sizeof(double));

  // Initiate the vector with the first token
  int enc = tk_encode(token_map, tokens->data[0]);
  double *first = embedding_retrieve(embedding_matrix, enc);

  // Initiate the values from the first vector
  for (int i = 0; i < (int)EMBEDDING_SIZE; i++) {
    emb_vec[i] = first[i];
  }

  for (size_t i = 1; i < tokens->size; i++) {
    enc = tk_encode(token_map, tokens->data[i]);
    double *tk_vec = embedding_retrieve(embedding_matrix, enc);
    // summing the values
    for (int j = 0; j < (int)EMBEDDING_SIZE; j++) {
      emb_vec[j] = emb_vec[j] + tk_vec[j];
    }
  }

  // divide by the number of tokens (averaging)
  for (int j = 0; j < (int)EMBEDDING_SIZE; j++) {
    emb_vec[j] = emb_vec[j] / tokens->size;
  }

  matrix *vec = matrix_to_col_vec(emb_vec, EMBEDDING_SIZE);
  return vec;
}
