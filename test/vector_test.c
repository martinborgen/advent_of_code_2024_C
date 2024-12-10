
#include <stdlib.h>
#include <stdio.h>
#include "vector.h"

int main()
{
    struct int_vector test_vec = int_vector_new();

    if (test_vec.capacity != 0 || test_vec.length != 0 || test_vec.values != NULL)
    {
        printf("Error in generating empty vector");
        return -1;
    }

    printf("hello again and again testing world!");
    return 0;
}