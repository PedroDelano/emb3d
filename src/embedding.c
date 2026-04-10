/*********************************
 *
 * Simple embedding implementation
 *
 *********************************/

#include "embedding.h"
#include "tokenizer.h"
#include <assert.h>

const unsigned int EMBEDDING_SIZE = 1024;

matrix *embedding_matrix_new(unsigned int vocab_size) {
  return matrix_new_rand(vocab_size, EMBEDDING_SIZE);
}

double *embedding_retrieve(matrix *embedding_matrix, int token_id) {
  assert(token_id < (int)embedding_matrix->num_rows &&
         "ERROR: invalid token_id (greater than vocab_size)");
  return embedding_matrix->data[token_id];
}

matrix *one_hot_encoding(int index, int size) {
  // Returns a [0, 0, 1, ..., 0] vector
  // where x[index] = 1
  matrix *m = matrix_new(1, size);
  m->data[0][index] = 1;
  return m;
}

matrix *embedding(matrix *embedding_matrix, Node **token_map, char *str) {

  /*
   * Simple embedding implementation via averaging
   * embedding vectors
   * */

  Array *tokens = tokenize(str);

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
