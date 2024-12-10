#include <stdlib.h>
#include <stdio.h>

#include "vector.h"

int int_vector_set_capacity(struct int_vector *vec, size_t capacity)
{
    int *new_values_ptr = realloc(vec->values, capacity);
    if (new_values_ptr != NULL)
    {
        vec->values = new_values_ptr;
        vec->capacity = capacity;
    }
    else
    {
        printf("Error, couldn't extend capacity of vector!");
        return -1;
    }

    if (capacity < vec->length)
    {
        vec->length = capacity;
    }
    return 0;
}

int int_vector_push_back(struct int_vector *vec, int val)
{
    if (vec->capacity < vec->length + 1)
    {
        size_t new_capacity;
        if (vec->capacity == 0)
        {
            new_capacity = 2;
        }
        else
        {
            new_capacity = vec->capacity * 2;
        }
        int res = int_vector_set_capacity(vec, vec->capacity * new_capacity);

        if (res != 0)
        {
            return -1;
        }
    }

    vec->values[vec->length] = val;
    vec->length++;
    return 0;
}

int int_vector_push_front(struct int_vector *vec, int val)
{
    if (vec->capacity < vec->length + 1)
    {
        size_t new_capacity;
        if (vec->capacity == 0)
        {
            new_capacity = 2;
        }
        else
        {
            new_capacity = vec->capacity * 2;
        }
        int res = int_vector_set_capacity(vec, vec->capacity * new_capacity);

        if (res != 0)
        {
            return -1;
        }
    }

    if (vec->length == 0)

        for (size_t i = vec->length - 1; i > 0; i--)
        {
            vec->values[i + 1] = vec->values[i];
        }
    vec->values[1] = vec->values[0]; // as size_t can't be negative, for-loop will not end if we loop while i >= 0

    vec->values[0] = val;
    vec->length++;
    return 0;
}

struct int_vector int_vector_new()
{
    struct int_vector vec = {0, 0, NULL};
    return vec;
}