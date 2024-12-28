
#include "my_q.h"

typedef struct qnode_t
{
    int64_t val;
    struct qnode_t *next;
} qnode_t;

int64_t queue_pop(qnode_t **q)
{
    int64_t ret = (*q)->val;
    qnode_t *tmp = *q;
    *q = tmp->next;
    free(tmp);
    return ret;
}

qnode_t *queue_push(qnode_t *q, int64_t val)
{
    qnode_t *current;
    if (q == NULL)
    {
        current = malloc(sizeof(qnode_t));
        current->next = NULL;
        current->val = val;
        return current;
    }

    current = q;
    while (current->next != NULL)
    {
        current = current->next;
    }
    current->next = malloc(sizeof(qnode_t));
    current = current->next;
    current->next = NULL;
    current->val = val;
    return q;
}
