
#include <stdbool.h>
#include <stdint.h>

#include "my_linalg.h"

quota quota_mult(quota a, quota b)
{
    quota res = {a.numerator * b.numerator, a.denominator * b.denominator};
    if (quota_is_int(res))
    {
        res = (quota){quota_icompute(res), 1};
    }
    return res;
}

quota quota_div(quota a, quota b)
{
    quota res = {a.numerator * b.denominator, a.denominator * b.numerator};
    if (quota_is_int(res))
    {
        res = (quota){quota_icompute(res), 1};
    }
    return res;
}

quota quota_add(quota a, quota b)
{
    quota res = {a.numerator * b.denominator + b.numerator * a.denominator,
                 a.denominator * b.denominator};
    if (quota_is_int(res))
    {
        res = (quota){quota_icompute(res), 1};
    }
    return res;
}

quota quota_sub(quota a, quota b)
{
    quota res = {a.numerator * b.denominator - b.numerator * a.denominator,
                 a.denominator * b.denominator};
    if (quota_is_int(res))
    {
        res = (quota){quota_icompute(res), 1};
    }
    return res;
}

float quota_fcompute(quota q)
{
    float res = (float)q.numerator / (float)q.denominator;
    return res;
}

int_fast64_t quota_icompute(quota q)
{
    int_fast64_t res = q.numerator / q.denominator;
    return res;
}

bool quota_is_int(quota q)
{
    return q.numerator % q.denominator == 0;
}