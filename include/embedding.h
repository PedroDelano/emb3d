#ifndef EMBEDDING_H
#define EMBEDDING_H

#include "matrix.h"
#include "tokenizer.h"

extern const unsigned int EMBEDDING_SIZE;

matrix *embedding_matrix_new(unsigned int vocab_size);
double *embedding_retrieve(matrix *embedding_matrix, int token_id);
matrix *embedding(matrix *embedding_matrix, Node **token_map, char *str);

#endif
