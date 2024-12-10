
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "vector.h"

int main()
{
    int output = 0;
    struct int_vector test_vec = int_vector_new();

    if (test_vec.capacity != 0 || test_vec.length != 0 || test_vec.values != NULL)
    {
        printf("Error in generating empty vector\n");
        output--;
    }

    int res_push_back_1 = int_vector_push_back(&test_vec, 1);
    if (res_push_back_1 != 0 || test_vec.capacity != 2 || test_vec.length != 1 || test_vec.values[0] != 1)
    {
        printf("Error in push_back on empty vector\n");
        output--;
    }

    int res_push_back_2 = int_vector_push_back(&test_vec, 2);
    if (res_push_back_2 != 0 || test_vec.capacity != 2 || test_vec.length != 2 || test_vec.values[0] != 1 || test_vec.values[1] != 2)
    {
        printf("Error on push_back on vector with one element in it\n");
        output--;
    }

    int res_push_front_1 = int_vector_push_front(&test_vec, 0);
    if (res_push_front_1 != 0 || test_vec.capacity != 4 || test_vec.length != 3 || test_vec.values[0] != 0 || test_vec.values[test_vec.length - 1] != 2)
    {
        printf("Error on push_front on vector with two element in it\n");
        output--;
    }

    bool res_search_1 = int_vector_contains(&test_vec, 2);
    if (res_search_1 == false)
    {
        printf("Error in contains, expected true but got false\n");
        output--;
    }

    bool res_search_2 = int_vector_contains(&test_vec, 8);
    if (res_search_2 == true)
    {
        printf("Error in contains, expected false but got true\n");
        output--;
    }

    size_t res_get_index_of_1 = int_vector_get_index_of(&test_vec, 1);
    if (res_get_index_of_1 != 1)
    {
        printf("Error in getting index of 1 which should exist at index 1\n");
        output--;
    }

    size_t res_get_index_of_12 = int_vector_get_index_of(&test_vec, 12);
    if (res_get_index_of_12 != __SIZE_MAX__)
    {
        printf("Error in getting index of 12 which should have returned SIZE_MAX\n");
        output--;
    }

    int res_resize_to_1 = int_vector_set_capacity(&test_vec, 1);
    if (res_resize_to_1 != 0 || test_vec.capacity != 1 || test_vec.length != 1 || test_vec.values[0] != 0)
    {
        printf("Error, resizing down to size 1 not behaving as expected\n");
        output--;
    }

    int res_resize_to_0 = int_vector_set_capacity(&test_vec, 0);
    if (res_resize_to_0 != 0 || test_vec.capacity != 0 || test_vec.length != 0)
    {
        printf("Error, resizing down to size 0 not behaving as expected\n");
        output--;
    }

    bool res_search_3 = int_vector_contains(&test_vec, 8);
    if (res_search_3 == true)
    {
        printf("Error in contains, expected false but got true\n");
    }

    int push_front_2 = int_vector_push_front(&test_vec, 9);
    if (push_front_2 != 0 || test_vec.capacity != 2 || test_vec.length != 1 || test_vec.values[0] != 9)
    {
        printf("Error when pushing front on empty vector");
        output--;
    }

    if (output == 0)
    {
        printf("Vector testing completed sucessfully\n");
    }
    else
    {
        printf("Vector testing failed %d tests!\n", -1 * output);
    }
    return output;
}