#ifndef HEADER_MATRIX_H
#define HEADER_MATRIX_H

#include <stddef.h>

int **dynamic_matrix_alloc_wrapper(unsigned long long N, unsigned long long M);
int **dynamic_matrix_alloc(size_t N, size_t M);
void dynamic_matrix_free(int **A, size_t N);
void print_matrix(int **A, size_t N, size_t M);

#endif
