#include <stdio.h>
#include <ctype.h>
#include <limits.h>

#include "return_codes.h"
#include "csv.h"

int csv_count_lines_and_columns(FILE *input,
                                unsigned long long *n_lines_csv,
                                unsigned long long *n_columns_csv)
{
    unsigned long long n_lines = 0;

    unsigned long long n_commas = 0;
    unsigned long long n_commas_required = 0;

    int old_symb;
    int symb = 0;

    for(;;)
    {
        old_symb = symb;
        symb = getc(input);
        if (symb == EOF)
        {
            if (old_symb != '\n')
            {
                n_lines++;
                if ((n_commas != n_commas_required) && (n_lines > 1))
                {
                    *n_lines_csv = n_lines;
                    return RC_BAD_FILE_FORMAT;
                }
            }
            break;
        }
        switch(symb)
        {
            case '\n':
                n_lines++;
                if ((n_commas != n_commas_required) && (n_lines > 1))
                {
                    *n_lines_csv = n_lines;
                    return RC_BAD_FILE_FORMAT;
                }
                n_commas_required = n_commas;
                n_commas = 0;
                break;
            case ',':
                n_commas++;
                break;
        }
    }
    *n_columns_csv = n_commas_required + 1;
    *n_lines_csv = n_lines;

    return 0;
}


int csv_calc_column_codes(FILE *input, int *column_codes, size_t *err_column)
{
    int symb;
    unsigned int column_code = 0;

    size_t i = 0;

    for(;;)
    {
        symb = getc(input);
        if (symb == EOF || symb == '\n')
        {
            column_codes[i] = (int)column_code;
            break;
        }
        else if (isalpha(symb))
        {
            column_code += symb;
        }
        else if (symb == ',')
        {
            column_codes[i] = (int)column_code;
            column_code = 0;
            i++;
        }
        else
        {
            *err_column = i + 1;
            return RC_BAD_FILE_FORMAT;
        }
    }

    return 0;
}
