#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <limits.h>

#include "return_codes.h"
#include "math_ops.h"
#include "parse.h"
#include "csv_matrix.h"


int read_csv_matrix(FILE *input, int **A, size_t *err_line, size_t *err_column)
{
    int err;

    size_t i = 0;
    size_t j = 0;

    int number;
    int symb = 0;

    for(;;)
    {
        symb = getc(input);
        if (symb == EOF)
        {
            break;
        }
        else if (symb == '=') // skip expression
        {
            do {

                symb = getc(input);

            } while(symb != ',' && (symb != '\n') && (symb != EOF));

            A[i][j] = CSV_MATRIX_UNDEFINED;
        }
        else
        {
            ungetc(symb, input);
            err = read_number(input, &number);
            if(!err && number == CSV_MATRIX_UNDEFINED)
            {
                err = RC_OVERFLOW;
            }
            switch(err)
            {
                case 0:
                    A[i][j] = number;
                    break;
                default:
                    *err_line = i + 1;
                    *err_column = j + 1;
                    return err;
            }
            symb = getc(input);
        }

        if (symb == EOF)
        {
            break;
        }

        switch(symb)
        {
            case '\n':
                i++;
                j = 0;
                break;
            case ',':
                j++;
                break;
            default:
                *err_line = i + 1;
                *err_column = j + 1;
                return RC_BAD_FILE_FORMAT;
        }
    }
    return 0;
}


int calc_expression(int math_op, int arg_1, int arg_2, int *result)
{
    int err;
    switch(math_op)
    {
        case '+' :
            err = sum(&arg_1, arg_2);
            if(err)
                return err;
            *result = arg_1;
            break;
        case '-':
            err = sub(&arg_1, arg_2);
            if(err)
                return err;
            *result = arg_1;
            break;
        case '*':
            err = mult(&arg_1, arg_2);
            if(err)
                return err;
            *result = arg_1;
            break;
        case '/':
            if (arg_2 == 0)
            {
                return RC_DIV_BY_ZERO;
            }
            else if (arg_1 == INT_MIN && arg_2 == -1)
            {
                return RC_OVERFLOW;
            }
            else
            {
                *result = arg_1 / arg_2;
            }
            break;
    }

    return 0;
}

int find_field(int **A, size_t n_lines, size_t n_columns,
               int line_number, unsigned int column_code, int *field)
{
    bool line_found = false;
    size_t i;
    for(i = 0; i < n_lines; i++)
    {
        if(A[i][0] == line_number)
        {
            line_found = true;
            break;
        }
    }
    if(!line_found)
    {
        return RC_NOT_FOUND;
    }

    bool column_found = false;
    size_t j;
    for(j = 0; j < n_columns; j++)
    {
        if(A[0][j] == (int)column_code)
        {
            column_found = true;
            break;
        }
    }
    if (!column_found)
    {
        return RC_NOT_FOUND;
    }

    if (A[i][j] == CSV_MATRIX_UNDEFINED)
    {
        return RC_BAD_REFERENCE;
    }

    *field = A[i][j];
    return 0;
}

int read_arg(FILE *input, int **A, size_t n_lines, size_t n_columns, int *arg)
{
    int err;
    int symb = getc(input);
    if (isalpha(symb))
    {
        unsigned int column_code = symb;
        symb = getc(input);
        if(!isdigit(symb))
        {
            while(isalpha(symb))
            {
                column_code += symb;
                symb = getc(input);
            }
        }
        ungetc(symb, input);
        int line_number;
        err = read_number(input, &line_number);
        if(err == RC_OVERFLOW)
            err = RC_TOO_BIG;
        if(err)
            return err;

        err = find_field(A, n_lines, n_columns, line_number, column_code, arg);
        if(err)
            return err;
    }
    else
    {
        ungetc(symb, input);
        err = read_number(input, arg);
        if(err == RC_OVERFLOW)
            err = RC_TOO_BIG;
        if(err)
            return err;
    }

    return 0;
}

int calc_csv_matrix(FILE *input, int **A, size_t n_lines, size_t n_columns, size_t *err_line, size_t *err_column)
{
    int err;

    int symb;

    int arg1;
    int arg2;
    int math_op;

    size_t i = 0;
    size_t j = 0;

    for (;;)
    {
        symb = getc(input);
        switch (symb)
        {
        case '=':
            err = read_arg(input, A, n_lines, n_columns, &arg1);
            if (err) {
                *err_line = i + 1;
                *err_column = j + 1;
                return err;
            }
            math_op = getc(input);
            if ((math_op != '+') && (math_op != '-') && (math_op != '*') && (math_op != '/'))
            {
                *err_line = i + 1;
                *err_column = j + 1;
                return RC_BAD_FILE_FORMAT;
            }

            err = read_arg(input, A, n_lines, n_columns, &arg2);
            if (err)
            {
                *err_line = i + 1;
                *err_column = j + 1;
                return err;
            }
            err = calc_expression(math_op, arg1, arg2, &A[i][j]);
            if(err)
            {
                *err_line = i + 1;
                *err_column = j + 1;
                return err;
            }
            symb = getc(input);
            break;
        default:
            while (symb != ',' && symb != '\n' && symb != EOF)
            {
                symb = getc(input);
            }
        }
        if (symb == EOF)
        {
            break;
        }

        switch (symb)
        {
            case ',':
                ++j;
                break;
            case '\n':
                ++i;
                j = 0;
                break;
            default:
                //printf("symb = %c and %d\n", symb, symb);
                *err_line = i + 1;
                *err_column = j + 1;
                return RC_BAD_FILE_FORMAT;
        }
    }

    return 0;
}


void print_csv_matrix(FILE *input, int **A, size_t num_lines, size_t num_columns)
{
    int symb = getc(input);;
    while((symb != EOF) && (symb != '\n'))
    {
        putchar(symb);
        symb = getc(input);
    }
    putchar('\n');
    for(size_t i = 1; i < num_lines; i++)
    {
        size_t j;
        for(j = 0; j < num_columns - 1; j++)
        {
            printf("%d,", A[i][j]);
        }
        printf("%d", A[i][j]);
        putchar('\n');
    }
}