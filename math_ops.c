#include <limits.h>

#include "return_codes.h"
#include "math_ops.h"


int mult(int *n, int b)
{
    int a = *n;
    if (a == 0 || b == 0)
    {
        *n = 0;
    }
    if (a > 0)
    {
        if (b > 0)
        {
            if(a <= INT_MAX / b)
            {
                *n = a * b;
            }
            else
            {
                return RC_OVERFLOW;
            }
        }
        else
        {
            if(b >= INT_MIN / a)
            {
                *n = a * b;
            }
            else
            {
                return RC_OVERFLOW;
            }
        }
    }
    else
    {
        if (b > 0)
        {
            if(a >= INT_MIN / b)
            {
                *n = a * b;
            }
            else
            {
                return RC_OVERFLOW;
            }
        }
        else
        {
            if(a >= INT_MAX / b)
            {
                *n = a * b;
            }
            else
            {
                return RC_OVERFLOW;
            }
        }
    }

    return 0;
}


int sum(int *n, int b)
{
    int a = *n;
    if (b >= 0)
    {
        if(a <= INT_MAX - b)
        {
            *n = a + b;
        }
        else
        {
            return RC_OVERFLOW;
        }
    }
    else
    {
        if(a >= INT_MIN - b)
        {
            *n = a + b;
        }
        else
        {
            return RC_OVERFLOW;
        }
    }
    return 0;
}


int sub(int *n, int b)
{
    int a = *n;
    if (b <= 0)
    {
        if(a <= INT_MAX + b)
        {
            *n = a - b;
        }
        else
        {
            return RC_OVERFLOW;
        }
    }
    else
    {
        if(a >= INT_MIN + b)
        {
            *n = a - b;
        }
        else
        {
            return RC_OVERFLOW;
        }
    }
    return 0;
}