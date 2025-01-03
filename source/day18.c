/*
https://adventofcode.com/2024/day/18

Part 1: find after 1024 bytes have fallen down

Part 2: find first byte that cuts off path

 */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file_reader.h"
#include "my_string.h"
#include "my_linalg.h"

// NOTE: Change SIDE_LENGTH when switching between sample and real input!

#define INPUTS_PATH "../inputs/day18.txt"
#define SIDE_LENGTH 71  // It is 7 for sample, 71 for real
#define TIME_PART1 1024 // It is 12 for sample, 1024 for real

typedef struct q_node
{
    tuple pos;
    struct q_node *next;
    int64_t cost;
} q_node;

typedef struct board_struct
{
    size_t rows, cols;
    char *maze;
    uint32_t *cost;
    tuple start;
    tuple end;
} board_t;

int64_t cost_estinmate(tuple pos)
{
    return 2 * (SIDE_LENGTH - 1) - pos.x - pos.y; // this ensures cost is admissable, i.e. a lower bound
}

bool priq_contains(q_node *q, tuple pos)
{
    while (q != NULL)
    {
        if (tuple_eq(q->pos, pos))
        {
            return true;
        }
        q = q->next;
    }
    return false;
}

void priq_insert(q_node **q, tuple pos)
{
    q_node *new_node = malloc(sizeof(q_node));
    new_node->pos = pos;
    new_node->cost = cost_estinmate(new_node->pos);

    if (*q == NULL)
    {
        new_node->next = NULL;
        *q = new_node;
    }
    else if (new_node->cost <= (*q)->cost)
    {
        new_node->next = *q;
        *q = new_node;
    }
    else
    {
        q_node *current = *q;
        q_node *prev = *q;
        while (current != NULL && new_node->cost > current->cost)
        {
            prev = current;
            current = current->next;
        }

        prev->next = new_node;
        new_node->next = current;
    }
}

tuple priq_pop(q_node **q)
{
    tuple ret = (*q)->pos;
    q_node *tmp = *q;
    *q = (*q)->next;
    free(tmp);
    return ret;
}

void get_neighbours(tuple here, q_node **prioq, board_t *board)
{

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
        if (cost_from_here < look_cost)
        {
            board->cost[look_pos.y * board->rows + look_pos.x] = cost_from_here;
            if (!priq_contains(*prioq, look_pos))
            {
                priq_insert(prioq, look_pos);
            }
        }
    }
}

uint32_t a_star(board_t *board)
{
    q_node *prioq = NULL;
    priq_insert(&prioq, board->start);
    while (prioq != NULL)
    {
        tuple here = priq_pop(&prioq);
        get_neighbours(here, &prioq, board);
    }
    uint32_t res = board->cost[board->end.y * board->rows + board->end.x];
    return res;
}

// Note: this assumes time from start. It will do all inputs from start to time.
// returns a pointer to the still unprocessed inputs
char *calc_falling_data(size_t time, char *inputs, board_t *board)
{
    char *token = strtok(inputs, "\n");
    for (size_t i = 0; i < time; i++)
    {
        size_t x, y;
        sscanf(token, "%lu,%lu", &x, &y);
        board->maze[y * SIDE_LENGTH + x] = '#';
        token = strtok(NULL, "\n");
        if (token == NULL)
        {
            break;
        }
    }
    token[strlen(token)] = '\n'; // strtok modifies string once too much
    return token;
}

void print_board(board_t *board)
{
    for (size_t i = 0; i < board->rows; i++)
    {
        for (size_t j = 0; j < board->cols; j++)
        {
            printf("%c", board->maze[i * board->rows + j]);
        }
        printf("\n");
    }
    printf("\n");
}

int main()
{
    char *inputs = file_reader(INPUTS_PATH);
    size_t time_max = strcnt(inputs, "\n");

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

    print_board(&board);

    // Part 1:
    char *inputs_part2 = calc_falling_data(TIME_PART1, inputs, &board); // not overwriting inputs* ptr so it can be used for free() later
    print_board(&board);
    uint32_t res_part1 = a_star(&board);

    printf("res part 1 is: %d\n", res_part1);

    // Part 2:
    char last_added_coords[6]; // because how part 2 answer should be give, this needs to keep track of what we added.

    uint32_t res_part2 = 0;
    uint32_t time_part2 = 0;
    for (size_t time = TIME_PART1; time < time_max - 1; time++)
    {
        // reset costs, keep board otherwise
        for (size_t i = 0; i < SIDE_LENGTH; i++)
        {
            for (size_t j = 0; j < SIDE_LENGTH; j++)
            {
                costs[i * SIDE_LENGTH + j] = UINT32_MAX;
            }
        }

        strncpy(last_added_coords, inputs_part2, 6);
        inputs_part2 = calc_falling_data(1, inputs_part2, &board); // due to how inputs are processed, increment of 1 instead of absolute time
        print_board(&board);
        res_part2 = a_star(&board);
        printf("Longest for time %lu is %u\n", time, res_part2);

        if (res_part2 == UINT32_MAX)
        {
            time_part2 = time;
            break;
        }
    }

    printf("time part 2 is : %d\n", time_part2);
    printf("The last coords to be added were:\n%s", last_added_coords);
    free(inputs);
    return 0;
}