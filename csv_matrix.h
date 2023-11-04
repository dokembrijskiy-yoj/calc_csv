#ifndef HEADER_PROCESS_H
#define HEADER_PROCESS_H

#include <stdio.h>
#include <limits.h>

#define CSV_MATRIX_MIN (INT_MIN + 1)
#define CSV_MATRIX_MAX INT_MAX
#define CSV_MATRIX_UNDEFINED INT_MIN


int read_csv_matrix(FILE *input, int **A, size_t *err_line, size_t *err_column);


int calc_expression(int math_op, int arg_1, int arg_2, int *result);


int find_field(int **A, size_t n_lines, size_t n_columns,
               int line_number, unsigned int column_code, int *field);


int read_arg(FILE *input, int **A, size_t n_lines, size_t n_columns, int *arg);


int calc_csv_matrix(FILE *input, int **A, size_t n_lines, size_t n_columns,
            size_t *err_line, size_t *err_column);


void print_csv_matrix(FILE *input, int **A, size_t num_lines, size_t num_columns);


#endif