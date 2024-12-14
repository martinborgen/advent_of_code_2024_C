
#include <stdbool.h>
#include <stdint.h>

#include "my_linalg.h"

quota quota_mult(quota a, quota b)
{
    quota res = {a.numerator * b.numerator, a.denominator * b.denominator};
    return res;
}

quota quota_div(quota a, quota b)
{
    quota res = {a.numerator * b.denominator, a.denominator * b.numerator};
    return res;
}

quota quota_add(quota a, quota b)
{
    quota res = {a.numerator * b.denominator + b.numerator * a.denominator,
                 a.denominator * b.denominator};
    return res;
}

quota quota_sub(quota a, quota b)
{
    quota res = {a.numerator * b.denominator - b.numerator * a.denominator,
                 a.denominator * b.denominator};
    return res;
}

float quota_fcompute(quota q)
{
    float res = (float)q.numerator / (float)q.denominator;
    return res;
}

int quota_icompute(quota q)
{
    int res = q.numerator / q.denominator;
    return res;
}

bool quota_is_int(quota q)
{
    return q.numerator % q.denominator == 0;
}