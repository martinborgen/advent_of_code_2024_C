
// #include <stdint.h>
// #include <stdlib.h>
// #include <stdio.h>
// #include <stdbool.h>

#include "my_hash_table.h"

typedef struct hash_node
{
    uint64_t val;
    uint64_t key;
    struct hash_node *next;
} hash_node;

void hash_table_insert(uint64_t key, uint64_t val, size_t hash_table_rows, hash_node *hash_table[hash_table_rows])
{
    size_t hash = key % hash_table_rows;
    hash_node *node = hash_table[hash];

    while (node != NULL && node->next != NULL && node->key != key)
    {
        node = node->next;
    }

    if (node == NULL)
    {
        hash_table[hash] = malloc(sizeof(hash_node));
        node = hash_table[hash];
    }
    else if (node != NULL && node->next == NULL)
    {
        node->next = malloc(sizeof(hash_node));
        node = node->next;
    }
    else if (key == node->key)
    {
        node->val = val;
        return;
    }

    node->key = key;
    node->val = val;
    node->next = NULL;
}

uint64_t hash_table_get(uint64_t key, size_t hash_table_rows, hash_node *hash_table[hash_table_rows])
{
    size_t hash = key % hash_table_rows;
    hash_node *node = hash_table[hash];

    while (node != NULL && node->key != key)
    {
        node = node->next;
    }

    if (node == NULL)
    {
        return 0;
    }
    else if (key == node->key)
    {
        return node->val;
    }
    else
    {
        printf("Hash get error, this shouldn't happen\n");
        return 0;
    }
}

bool hash_table_contains(uint64_t key, size_t hash_table_size, hash_node **hash_table)
{
    hash_node *node = hash_table[key % hash_table_size];
    while (node != NULL && node->key != key)
    {
        node = node->next;
    }

    return node != NULL;
}

hash_node **hash_table_calloc_new(size_t hash_table_size)
{
    hash_node **hashtable = malloc(sizeof(hash_node *) * hash_table_size); // 1009 is just a prime number of a hunch
    for (int i = 0; i < hash_table_size; i++)
    {
        hashtable[i] = NULL;
    }
    return hashtable;
}

void hash_table_clear(size_t hash_table_size, hash_node **hash_table)
{

    for (int i = 0; i < hash_table_size; i++)
    {
        hash_node *current = hash_table[i];
        hash_node *next = NULL;
        while (current != NULL)
        {
            next = current->next;
            free(current);
            current = next;
        }
        hash_table[i] = NULL;
    }
}