
#ifndef _AOC_HASH_TABLE_H
#define _AOC_HASH_TABLE_H

typedef struct hash_node hash_node;

/*
For uint64_t only!
Hash table should just be an array of hash_node*
 */
void hash_map_insert(uint64_t key, uint64_t val, size_t hash_table_rows, hash_node *hash_table[hash_table_rows]);

/*
For uint64_t only!
Hash table should just be an array of hash_node*
 */
uint64_t hash_map_get(uint64_t key, size_t hash_table_rows, hash_node *hash_table[hash_table_rows]);

#endif