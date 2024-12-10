#ifndef _AOC_VECTOR_H
#define _AOC_VECTOR_H

/*
An heap-allocated array, bundled with its' length and capacity for convenience.
`size_t length` - The number of elements that has been pushed to the vector.
`size_t capacity` - The number of elements the `values` can hold before it has to re-allocate
`int *values` - a heap allocated array. The associated functions in this
header might `re-alloc()` this array, so don't save this pointer!
 */
struct int_vector
{
    size_t length;
    size_t capacity;
    int *values;
};

/* Sets a capacity of vector. Length remains unchanged,
unless the new size means some elements are discared.
Returns 0 if successful.
 */
int int_vector_set_capacity(struct int_vector *vec, size_t capacity);

/* Pushes an element to the back of the vector.
Will extend the vector by doubling its capacity if it is out of space
returns 0 if successful. */
int int_vector_push_back(struct int_vector *vec, int val);

/* Pushes an element to the front of the vector, shifting all pre-existing elements by one.
Will extend the vector by doubling its capacity if it is out of space
returns 0 if successful. */
int int_vector_push_front(struct int_vector *vec, int val);

/* Returns a vector with zero elements */
struct int_vector int_vector_new();

#endif