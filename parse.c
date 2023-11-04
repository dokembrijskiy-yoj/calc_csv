#include <stdio.h>
#include <ctype.h>

#include "return_codes.h"
#include "math_ops.h"
#include "parse.h"


int read_number(FILE *input, int *number)
{
    int err_mult;
    int err_sum;
    int err_sub;

    int symb;
    int num = 0;

    symb = getc(input);
    if (symb == '-')
    {
        symb = getc(input);
        if(!isdigit(symb))
        {
            return RC_BAD_FILE_FORMAT;
        }
        num = -(symb - '0');
        symb = getc(input);
        while((symb != EOF) && isdigit(symb))
        {
            err_mult = mult(&num, 10);
            err_sub = sub(&num, (symb - '0'));
            if(err_mult || err_sub)
            {
                return RC_OVERFLOW;
            }
            symb = getc(input);
        }

        ungetc(symb, input);
    }
    else if (isdigit(symb))
    {
        num = symb - '0';
        symb = getc(input);
        while((symb != EOF) && isdigit(symb))
        {
            err_mult = mult(&num, 10);
            err_sum = sum(&num, (symb - '0'));
            if(err_mult || err_sum)
            {
                return RC_OVERFLOW;
            }
            symb = getc(input);
        }

        ungetc(symb, input);
    }
    else if (symb == EOF)
    {
        return RC_NO_INPUT;
    }
    else
    {
        return RC_BAD_FILE_FORMAT;
    }

    *number = num;
    return 0;
}