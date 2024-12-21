#ifndef _AOC_LINALG_H
#define _AOC_LINALG_H

#include <stdint.h>

typedef struct quota
{
    int_fast64_t numerator, denominator;
} quota;

typedef struct tuple
{
    int64_t x, y;
} tuple;

quota quota_mult(quota a, quota b);

quota quota_div(quota a, quota b);

quota quota_add(quota a, quota b);

quota quota_sub(quota a, quota b);

float quota_fcompute(quota q);

int_fast64_t quota_icompute(quota q);

bool quota_is_int(quota q);

tuple tuple_add(tuple a, tuple b);

tuple tuple_sub(tuple a, tuple b);

tuple tuple_neg(tuple a);

tuple tuple_scalar_mult(tuple a, int b);

int64_t tuple_dot_prod(tuple a, tuple b);

bool tuple_eq(tuple a, tuple b);

#endif