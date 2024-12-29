

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file_reader.h"
#include "my_string.h"
#include "my_linalg.h"

// NOTE: Change SIDE_LENGTH when switching between sample and real input!

#define INPUTS_PATH "../inputs/day18_sample.txt"
#define SIDE_LENGTH 7 // It is 7 for sample, 71 for real
#define TIME_PART1 12 // It is 12 for sample, 1024 for real

typedef struct q_node
{
    tuple pos;
    struct q_node *next;
} q_node;

typedef struct board_struct
{
    size_t rows, cols;
    char *maze;
    uint32_t *cost;
    tuple start;
    tuple end;
} board_t;

int64_t scorefun(tuple pos)
{
    return pos.x * pos.x + pos.y * pos.y;
}

void priq_insert(q_node **q, tuple pos)
{
    if (*q == NULL)
    {
        (*q) = malloc(sizeof(q_node));
        (*q)->next = NULL;
        (*q)->pos = pos;
        return;
    }

    int64_t pos_score = scorefun(pos);
    if (scorefun((*q)->pos) < pos_score)
    {
        q_node *tmp = *q;
        *q = malloc(sizeof(q_node));
        (*q)->next = tmp;
        (*q)->pos = pos;
        return;
    }

    q_node *current = *q;
    int64_t current_score = scorefun(current->pos);

    while (current->next != NULL && current_score > pos_score)
    {
        current = current->next;
        current_score = scorefun(current->pos);
    }

    q_node *tmp_next = current->next;
    current->next = malloc(sizeof(q_node));
    current->next->pos = pos;
    current->next->next = tmp_next;
}

tuple priq_pop(q_node **q)
{
    tuple ret = (*q)->pos;
    q_node *tmp = *q;
    *q = (*q)->next;
    free(tmp);
    return ret;
}

void bfs_search(q_node **prioq, board_t *board)
{
    tuple here = priq_pop(prioq);
    // if we're at the end
    if (tuple_eq(here, board->end))
    {
        return;
    }

    tuple look_dirs[] = {{0, 1},
                         {1, 0},
                         {0, -1},
                         {-1, 0}};

    for (size_t i = 0; i < sizeof(look_dirs) / sizeof(tuple); i++)
    {
        tuple look_pos = tuple_add(look_dirs[i], here);
        char look_char = board->maze[look_pos.y * board->rows + look_pos.x];
        if (look_pos.x < 0 || look_pos.y < 0 || look_pos.x >= SIDE_LENGTH || look_pos.y >= SIDE_LENGTH || look_char == '#')
        {
            continue;
        }

        uint32_t look_cost = board->cost[look_pos.y * board->rows + look_pos.x];
        uint32_t cost_from_here = board->cost[here.y * board->rows + here.x] + 1;
        if (cost_from_here <= look_cost)
        {
            board->cost[look_pos.y * board->rows + look_pos.x] = cost_from_here;
            priq_insert(prioq, look_pos);
        }
    }
}

// Note: this assumes time from start. It will do all inputs from start to time.
void calc_falling_data(size_t time, char *inputs, board_t *board)
{
    char *token = strtok(inputs, "\n");
    for (size_t i = 0; i < time; i++)
    {
        size_t x, y;
        sscanf(token, "%lu,%lu", &x, &y);
        board->maze[y * SIDE_LENGTH + x] = '#';
        token = strtok(NULL, "\n");
    }
}

void print_arr(size_t rows, size_t cols, char *arr)
{
    for (size_t i = 0; i < rows; i++)
    {
        for (size_t j = 0; j < cols; j++)
        {
            printf("%c", arr[i * rows + j]);
        }
        printf("\n");
    }
    printf("\n");
}

int main()
{
    char *inputs = file_reader(INPUTS_PATH);

    char *maze = malloc(sizeof(char) * SIDE_LENGTH * SIDE_LENGTH);
    uint32_t *costs = malloc(sizeof(uint32_t) * SIDE_LENGTH * SIDE_LENGTH);

    for (size_t i = 0; i < SIDE_LENGTH; i++)
    {
        for (size_t j = 0; j < SIDE_LENGTH; j++)
        {
            maze[i * SIDE_LENGTH + j] = '.';
            costs[i * SIDE_LENGTH + j] = UINT32_MAX;
        }
    }

    board_t board;
    board.rows = SIDE_LENGTH;
    board.cols = SIDE_LENGTH;
    board.cost = costs;
    board.start = (tuple){0, 0};
    board.end = (tuple){SIDE_LENGTH - 1, SIDE_LENGTH - 1};
    board.maze = maze;

    board.cost[board.start.y * board.rows + board.start.x] = 0;

    print_arr(board.rows, board.cols, board.maze);

    calc_falling_data(TIME_PART1, inputs, &board);

    print_arr(board.rows, board.cols, board.maze);

    q_node *prioq = NULL;
    priq_insert(&prioq, board.start);
    while (prioq != NULL)
    {
        printf("%ld,%ld\n", prioq->pos.y, prioq->pos.x);
        bfs_search(&prioq, &board);
    }
    uint32_t res = board.cost[board.end.y * board.rows + board.end.x];
    // print_board_w_visited(&board, visited);
    printf("res is: %d\n", res);
    return 0;
}