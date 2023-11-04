#include <stdio.h>
#include <stdlib.h>

#include "matrix.h"
#include "csv.h"
#include "return_codes.h"
#include "csv_matrix.h"

enum exit_status
{
    ES_SUCCESS = 0,
    ES_BAD_INPUT = 1,
    ES_ENV_ERROR = 2,
    ES_INTERNAL = 3
};

static const char *program_name = "csv_file";


void check_first_column(int **A, size_t num_lines, const char *file_name)
{

    for(size_t i = 1; i < num_lines; i++)
    {
        if (A[i][0] <= 0)
        {
            fprintf(stderr, "%s: %s: %zu: error in field #1: negative number.\n",
                    program_name, file_name, i);
            exit(ES_BAD_INPUT);
        }
        for(size_t n = i + 1; n < num_lines; n++)
        {
            if (A[i][0] == A[n][0])
            {
                fprintf(stderr, "%s: %s: line numbers %zu and %zu are the same.\n",
                        program_name, file_name, i, n);
                exit(ES_BAD_INPUT);
            }
        }
    }
}


void rewind_file(FILE *file, const char *file_name)
{
    int err = fseek(file, 0, SEEK_SET);
    if(err)
    {
        fprintf(stderr, "%s: %s: I/O error.\n", program_name, file_name);
        exit(ES_ENV_ERROR);
    }
}


int main(int argc, char *argv[])
{
    int err;
    int status = 0;

    if(argv[0] != NULL)
    {
        program_name = argv[0];
    }

    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s FILE\n", program_name);
        return ES_BAD_INPUT;
    }

    const char *const file_name = argv[1];

    FILE *const csv_file = fopen(file_name, "r");
    if (csv_file == NULL)
    {
        fprintf(stderr, "%s: Failed to open file \"%s\"\n",
                program_name, file_name);
        return ES_ENV_ERROR;
    }

    unsigned long long num_lines;
    unsigned long long num_columns;

    err = csv_count_lines_and_columns(csv_file, &num_lines, &num_columns);
    if(err)
    {
        fprintf(stderr, "%s: %s: %llu: Incorrect number of fields.\n",
                program_name, file_name, num_lines);
        return ES_BAD_INPUT;
    }

    rewind_file(csv_file, file_name);


    int **A = dynamic_matrix_alloc_wrapper(num_lines, num_columns);
    if (A == NULL)
    {
        fprintf(stderr, "%s: Memory allocation error.\n", program_name);
        return ES_ENV_ERROR;
    }

    size_t err_column;
    err = csv_calc_column_codes(csv_file, A[0], &err_column);
    if(A[0][0] != 0)
    {
        err = RC_BAD_FILE_FORMAT;
        err_column = 1;
    }
    if(err)
    {
        fprintf(stderr, "%s: %s: 1: error in field #%zu: syntax error.\n",
                program_name, file_name, err_column);
        status = ES_BAD_INPUT;
        goto end;
    }

    size_t err_line;
    err = read_csv_matrix(csv_file, A + 1, &err_line, &err_column);
    switch(err)
    {
        case 0:
            break;
        case RC_BAD_FILE_FORMAT:
            fprintf(stderr, "%s: %s: %zu: error in field #%zu: syntax error.\n",
                    program_name, file_name, err_line + 1, err_column);
            status = ES_BAD_INPUT;
            goto end;
        case RC_OVERFLOW:
            fprintf(stderr, "%s: %s: %zu: error in field #%zu: number is out of "
                    "range [%d,%d].\n", program_name, file_name, err_line + 1,
                    err_column, CSV_MATRIX_MIN, CSV_MATRIX_MIN);
            status = ES_ENV_ERROR;
            goto end;
     }

    check_first_column(A, num_lines, file_name);

    //print_matrix(A, num_lines, num_columns);

    rewind_file(csv_file, file_name);

    err = calc_csv_matrix(csv_file, A, num_lines, num_columns, &err_line, &err_column);
    switch(err)
    {
        case 0:
            break;
        case RC_BAD_FILE_FORMAT:
            fprintf(stderr, "%s: %s: %zu: error in field #%zu: syntax error.\n",
                    program_name, file_name, err_line, err_column);
            status = ES_BAD_INPUT;
            goto end;
        case RC_OVERFLOW:
            fprintf(stderr, "%s: %s: %zu: error in field #%zu: result is out of "
                    "range [%d,%d].\n", program_name, file_name, err_line,
                    err_column, CSV_MATRIX_MIN, CSV_MATRIX_MIN);
            status = ES_ENV_ERROR;
            goto end;
        case RC_TOO_BIG:
            fprintf(stderr, "%s: %s: %zu: error in field #%zu: number is out of "
                    "range [%d,%d].\n", program_name, file_name, err_line,
                    err_column, CSV_MATRIX_MIN, CSV_MATRIX_MIN);
            status = ES_ENV_ERROR;
            goto end;
        case RC_DIV_BY_ZERO:
            fprintf(stderr, "%s: %s: %zu: error in field #%zu: division by zero.\n",
                    program_name, file_name, err_line, err_column);
            status = ES_BAD_INPUT;
            goto end;
        case RC_NOT_FOUND:
            fprintf(stderr, "%s: %s: %zu: error in field #%zu: undefined reference.\n",
                    program_name, file_name, err_line, err_column);
            status = ES_BAD_INPUT;
            goto end;
        case RC_BAD_REFERENCE:
            fprintf(stderr, "%s: %s: %zu: error in field #%zu: reference to expression.\n",
                    program_name, file_name, err_line, err_column);
            status = ES_BAD_INPUT;
            goto end;
    }

    rewind_file(csv_file, file_name);

    print_csv_matrix(csv_file, A, num_lines, num_columns);

end:
    dynamic_matrix_free(A, num_lines);

    fclose(csv_file);

    return status;
}
