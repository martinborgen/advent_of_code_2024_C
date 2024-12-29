

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
#define SIDE_LENGTH 7

typedef struct board_struct
{
    size_t rows, cols;
    char *maze;
    uint32_t *cost;
    tuple start;
    tuple end;
} board_t;

uint32_t dfs_search(tuple here, uint32_t acc_cost, board_t *board, bool *visited)
{
    // if we're at the end
    if (tuple_eq(here, board->end))
    {
        board->cost[here.y * board->rows + here.x] = acc_cost;
        return acc_cost;
    }

    tuple look_dirs[] = {{-1, 0},
                         {1, 0},
                         {0, -1},
                         {0, 1}};

    visited[here.y * board->rows + here.x] = true;
    uint32_t min_fork = UINT32_MAX;
    // look up, down, left and right.
    for (size_t i = 0; i < sizeof(look_dirs) / sizeof(tuple); i++)
    {
        tuple look_pos = tuple_add(look_dirs[i], here);
        if (look_pos.x < 0 || look_pos.y < 0 || look_pos.x >= SIDE_LENGTH || look_pos.y >= SIDE_LENGTH)
        {
            continue;
        }

        char look_char = board->maze[look_pos.y * board->rows + look_pos.x];
        bool look_visited = visited[look_pos.y * board->rows + look_pos.x];

        if (look_char == '#' || look_visited)
        {
            continue;
        }

        uint32_t look_end_cost = board->cost[look_pos.y * board->rows + look_pos.x];
        uint32_t cost_from_here = acc_cost + 1;
        if (cost_from_here <= look_end_cost)
        {
            uint32_t fork_depth = dfs_search(look_pos, cost_from_here, board, visited);
            if (fork_depth <= board->cost[here.y * board->rows + here.x])
            {
                board->cost[here.y * board->rows + here.x] = fork_depth;
            }

            if (fork_depth <= min_fork)
            {
                min_fork = fork_depth;
            }
        }
    }
    visited[here.y * board->rows + here.x] = false;
    return min_fork;
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

void print_board_w_visited(board_t *board, bool *visited)
{
    for (size_t i = 0; i < board->rows; i++)
    {
        for (size_t j = 0; j < board->cols; j++)
        {
            if (visited[i * board->rows + j])
            {
                printf("%c", 'O');
            }
            else
            {
                printf("%c", board->maze[i * board->rows + j]);
            }
        }
        printf("\n");
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
    bool *visited = malloc(sizeof(bool) * SIDE_LENGTH * SIDE_LENGTH);

    for (size_t i = 0; i < SIDE_LENGTH; i++)
    {
        for (size_t j = 0; j < SIDE_LENGTH; j++)
        {
            maze[i * SIDE_LENGTH + j] = '.';
            costs[i * SIDE_LENGTH + j] = UINT32_MAX;
            visited[i * SIDE_LENGTH + j] = false;
        }
    }

    board_t board;
    board.rows = SIDE_LENGTH;
    board.cols = SIDE_LENGTH;
    board.cost = costs;
    board.start = (tuple){0, 0};
    board.end = (tuple){SIDE_LENGTH - 1, SIDE_LENGTH - 1};
    board.maze = maze;

    print_arr(board.rows, board.cols, board.maze);

    calc_falling_data(12, inputs, &board);

    print_arr(board.rows, board.cols, board.maze);

    uint32_t res = dfs_search(board.start, 0, &board, visited);

    print_board_w_visited(&board, visited);
    return 0;
}