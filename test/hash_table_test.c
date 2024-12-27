
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "my_hash_table.h"

int main()
{
    const size_t hash_table_rows = 1009;
    int output = 0;
    hash_node **table = malloc(sizeof(hash_node *) * hash_table_rows);
    for (int i = 0; i < hash_table_rows; i++)
    {
        table[i] = NULL;
    }

    uint64_t res1 = hash_table_get(2, hash_table_rows, table);
    if (res1 != 0)
    {
        printf("Get error on empty table!\n");
        output--;
    }

    hash_table_insert(2, 22, hash_table_rows, table);
    uint64_t res2 = hash_table_get(2, hash_table_rows, table);
    if (res2 != 22)
    {
        printf("set error on empty key 2 value 22\n");
        output--;
    }

    hash_table_insert(hash_table_rows + 2, 123, hash_table_rows, table);
    uint64_t res3 = hash_table_get(2, hash_table_rows, table);
    if (res3 != 22)
    {
        printf("get/set error on collision, got %lu, but expected 2\n", res3);
        output--;
    }

    uint64_t res4 = hash_table_get(hash_table_rows + 2, hash_table_rows, table);
    if (res4 != 123)
    {
        printf("get/set error on collision, got %lu, but expected 123\n", res4);
        output--;
    }

    bool res5 = hash_table_contains(hash_table_rows + 2, hash_table_rows, table);
    if (!res5)
    {
        printf("get/set error on contains, got false, but expected true");
        output--;
    }

    hash_table_clear(hash_table_rows, table);
}