#ifndef _AOC_LINALG_H
#define _AOC_LINALG_H

#include <stdint.h>

typedef struct quota
{
    int_fast64_t numerator, denominator;
} quota;

quota quota_mult(quota a, quota b);

quota quota_div(quota a, quota b);

quota quota_add(quota a, quota b);

quota quota_sub(quota a, quota b);

float quota_fcompute(quota q);

int_fast64_t quota_icompute(quota q);

bool quota_is_int(quota q);
#endif