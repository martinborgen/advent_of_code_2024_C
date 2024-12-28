
#ifndef AOC_MYQ_H
#define AOC_MYQ_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

// to make a new queue, use `queue_push`
typedef struct qnode_t qnode_t;

// returns val from front of queue, frees the node
// the **q ptr is modified to point to the next element
int64_t queue_pop(qnode_t **q);

// mallocs node with val to the back of q. If q == NULL, then mallocs new node
qnode_t *queue_push(qnode_t *q, int64_t val);

#endif