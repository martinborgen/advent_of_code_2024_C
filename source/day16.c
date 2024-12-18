/*

https://adventofcode.com/2024/day/16


It's time again for the Reindeer Olympics! This year, the big event is the
Reindeer Maze, where the Reindeer compete for the lowest score.

You and The Historians arrive to search for the Chief right as the event is
about to start. It wouldn't hurt to watch a little, right?

The Reindeer start on the Start Tile (marked S) facing East and need to reach
the End Tile (marked E). They can move forward one tile at a time (increasing
their score by 1 point), but never into a wall (#). They can also rotate
clockwise or counterclockwise 90 degrees at a time (increasing their score by
1000 points).

To figure out the best place to sit, you start by grabbing a map (your puzzle
input) from a nearby kiosk. For example:

    ###############
    #.......#....E#
    #.#.###.#.###.#
    #.....#.#...#.#
    #.###.#####.#.#
    #.#.#.......#.#
    #.#.#####.###.#
    #...........#.#
    ###.#.#####.#.#
    #...#.....#.#.#
    #.#.#.###.#.#.#
    #.....#...#.#.#
    #.###.#.#.#.#.#
    #S..#.....#...#
    ###############

There are many paths through this maze, but taking any of the best paths would
incur a score of only 7036. This can be achieved by taking a total of 36 steps
forward and turning 90 degrees a total of 7 times:


    ###############
    #.......#....E#
    #.#.###.#.###^#
    #.....#.#...#^#
    #.###.#####.#^#
    #.#.#.......#^#
    #.#.#####.###^#
    #..>>>>>>>>v#^#
    ###^#.#####v#^#
    #>>^#.....#v#^#
    #^#.#.###.#v#^#
    #^....#...#v#^#
    #^###.#.#.#v#^#
    #S..#.....#>>^#
    ###############

Here's a second example:

    #################
    #...#...#...#..E#
    #.#.#.#.#.#.#.#.#
    #.#.#.#...#...#.#
    #.#.#.#.###.#.#.#
    #...#.#.#.....#.#
    #.#.#.#.#.#####.#
    #.#...#.#.#.....#
    #.#.#####.#.###.#
    #.#.#.......#...#
    #.#.###.#####.###
    #.#.#...#.....#.#
    #.#.#.#####.###.#
    #.#.#.........#.#
    #.#.#.#########.#
    #S#.............#
    #################

In this maze, the best paths cost 11048 points; following one such path would
look like this:

    #################
    #...#...#...#..E#
    #.#.#.#.#.#.#.#^#
    #.#.#.#...#...#^#
    #.#.#.#.###.#.#^#
    #>>v#.#.#.....#^#
    #^#v#.#.#.#####^#
    #^#v..#.#.#>>>>^#
    #^#v#####.#^###.#
    #^#v#..>>>>^#...#
    #^#v###^#####.###
    #^#v#>>^#.....#.#
    #^#v#^#####.###.#
    #^#v#^........#.#
    #^#v#^#########.#
    #S#>>^..........#
    #################

Note that the path shown above includes one 90 degree turn as the very first
move, rotating the Reindeer from facing East to facing North.

Analyze your map carefully. What is the lowest score a Reindeer could possibly
get?
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file_reader.h"
#include "my_string.h"

#define INPUTS_PATH "../inputs/day16.txt"

// tuple for positions and directions. Hence the r and c names (for row and column)
// `tuple = {size_t r, size_t c}`
typedef struct tuple
{
    size_t r, c;
} tuple;

typedef struct board_struct
{
    size_t rows, cols;
    char *maze;
    uint32_t *cost;
    tuple start;
    tuple end;
} board_t;

tuple tuple_add(tuple a, tuple b)
{
    return (tuple){a.r + b.r, a.c + b.c};
}

tuple tuple_sub(tuple a, tuple b)
{
    return (tuple){a.r - b.r, a.c - b.c};
}

bool tuple_eq(tuple a, tuple b)
{
    return (a.r == b.r && a.c == b.c);
}

void dfs_search(tuple here, tuple prev, uint32_t acc_cost, board_t *board)
{
    tuple look_dirs[] = {{-1, 0},
                         {1, 0},
                         {0, -1},
                         {0, 1}};

    tuple moved_dir = tuple_sub(here, prev);

    if (board->cost[here.r * board->rows + here.c] > acc_cost)
    {
        board->cost[here.r * board->rows + here.c] = acc_cost;
    }

    for (size_t i = 0; i < sizeof(look_dirs) / sizeof(tuple); i++)
    {
        // note: no bounds checking, as the boundary is always a line of '#'
        tuple look_pos = tuple_add(look_dirs[i], here);
        char look_char = board->maze[look_pos.r * board->rows + look_pos.c];
        if (look_char == '#')
        {
            continue;
        }

        bool is_turn = !tuple_eq(look_dirs[i], moved_dir);
        uint32_t look_cost = board->cost[look_pos.r * board->rows + look_pos.c];
        uint32_t cost_from_here = acc_cost + 1 + (1000 * is_turn);

        if (cost_from_here < look_cost)
        {
            dfs_search(look_pos, here, cost_from_here, board);
        }
    }
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
}

int main()
{
    char *inputs = file_reader(INPUTS_PATH);

    size_t rows_n = strcnt(inputs, "\n");
    size_t cols_n = strstr(inputs, "\n") - inputs;

    char *maze = malloc(sizeof(char) * rows_n * cols_n);
    uint32_t *costs = malloc(sizeof(uint32_t) * rows_n * cols_n); // the number is the cost to get there
    tuple start = {0};
    tuple end = {0};

    char *line = inputs;
    for (int i = 0; i < rows_n; i++)
    {
        memcpy(maze + (i * cols_n), line, sizeof(char) * cols_n);
        line = strstr(line, "\n") + 1;

        for (int j = 0; j < cols_n; j++)
        {
            costs[i * rows_n + j] = __UINT32_MAX__;

            if (maze[i * rows_n + j] == 'S')
            {
                start.r = i;
                start.c = j;
            }
            else if (maze[i * rows_n + j] == 'E')
            {
                end.r = i;
                end.c = j;
            }
        }
    }

    board_t board;
    board.rows = rows_n;
    board.cols = cols_n;
    board.cost = costs;
    board.start = start;
    board.end = end;
    board.maze = maze;

    print_board(&board);

    dfs_search(start, (tuple){start.r, start.c - 1}, 0, &board);

    uint32_t final_cost = board.cost[end.r * board.rows + end.c];
    printf("Part 1. Final cost: %u\n", final_cost);

    free(inputs);
    inputs = NULL;
}