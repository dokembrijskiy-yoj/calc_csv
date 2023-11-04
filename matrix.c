#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "matrix.h"


int **dynamic_matrix_alloc(size_t N, size_t M)
{
    int **A = (int **)calloc(N, sizeof(int *));
    if (A == NULL)
    {
        return NULL;
    }
    for (size_t i = 0; i < N; i++)
    {
        A[i] = (int *)calloc(M, sizeof(int));
        if (A[i] == NULL)
        {
            for(size_t n = 0; n < i; n++)
            {
                free(A[n]);
            }
            free(A);
            return NULL;
        }
    }
    return A;
}


int **dynamic_matrix_alloc_wrapper(unsigned long long N, unsigned long long M)
{
    if ((N > SIZE_MAX) || (M > SIZE_MAX))
    {
        return NULL;
    }

    return dynamic_matrix_alloc((size_t)N, (size_t)M);
}


void dynamic_matrix_free(int **A, size_t N)
{
    for (size_t i = 0; i < N; ++i)
    {
        free(A[i]);
    }
    free(A);
}


void print_matrix(int **A, size_t N, size_t M)
{
    for (size_t i = 0; i < N; i++)
    {
        for (size_t j = 0; j < M; j++)
        {
            printf("%*d", 15, A[i][j]);
        }
        printf("\n");
    }
}