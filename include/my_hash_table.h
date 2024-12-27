
#ifndef _AOC_HASH_TABLE_H
#define _AOC_HASH_TABLE_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

/*
The hash table provided here is only an array of pointers to hash_node structs.
Since this table only supports `uint_64t`, the only hashing going on is taking `key % hash_table_size`
Since the structs form a linked list for collisions, use the dedicated `hash_table_clear()` function
 */
typedef struct hash_node hash_node;

/*
For uint64_t only!
Hash table should just be an array of hash_node*
 */
void hash_table_insert(uint64_t key, uint64_t val, size_t hash_table_rows, hash_node *hash_table[hash_table_rows]);

/*
For uint64_t only!
Hash table should just be an array of hash_node*
 */
uint64_t hash_table_get(uint64_t key, size_t hash_table_rows, hash_node *hash_table[hash_table_rows]);

/*
Returns whether the key is in the hash table.
 */
bool hash_table_contains(uint64_t key, size_t hash_table_size, hash_node **hash_table);

/*
clears the entire hash table, including the collisions lists.
 */
void hash_table_clear(size_t hash_table_size, hash_node **hash_table);

#endif