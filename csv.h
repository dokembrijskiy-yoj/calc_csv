#ifndef HEADER_CSV_H
#define HEADER_CSV_H

#include <stdio.h>


int csv_count_lines_and_columns(FILE *input,
                                unsigned long long *n_lines_csv,
                                unsigned long long *n_columns_csv);


int csv_calc_column_codes(FILE *input, int *column_codes, size_t *err_column);


#endif