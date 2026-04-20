#ifndef EMBEDDING_H
#define EMBEDDING_H

#include "matrix.h"
#include "tokenizer.h"

extern unsigned int EMBEDDING_SIZE;

void embedding_set_size(unsigned int size);
matrix *embedding_matrix_new(unsigned int vocab_size);
double sparcity_score(matrix *embedding_matrix, double epsilon);
double *embedding_retrieve(matrix *embedding_matrix, int token_id);
matrix *embedding(matrix *embedding_matrix, Node **token_map, Array *tokens);
matrix *one_hot_encoding(int index, int size);

#endif
