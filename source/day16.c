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

#define INPUTS_PATH "../inputs/day16_sample.txt"

// tuple for positions and directions. Hence the r and c names (for row and column)
// `tuple = {size_t r, size_t c}`
typedef struct tuple
{
    size_t r, c;
} tuple;

typedef struct edge
{
    uint32_t weight;
    struct maze_node *node;
} edge;

typedef struct maze_node
{
    tuple pos;
    struct edge up, down, left, right;
} node;

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

tuple tuple_neg(tuple a)
{
    return (tuple){-a.r, -a.c};
}

tuple tuple_mult(tuple a, tuple b)
{
    return (tuple){a.r * b.r, a.c * b.c};
}

bool tuple_eq(tuple a, tuple b)
{
    return (a.r == b.r && a.c == b.c);
}

node *node_malloc(tuple here)
{
    node *new_node = malloc(sizeof(node));

    new_node->pos = here;

    new_node->up.node = NULL;
    new_node->up.weight = UINT32_MAX;
    new_node->down.node = NULL;
    new_node->down.weight = UINT32_MAX;
    new_node->right.node = NULL;
    new_node->right.weight = UINT32_MAX;
    new_node->left.node = NULL;
    new_node->left.weight = UINT32_MAX;

    return new_node;
}

uint32_t dfs_search(tuple here, tuple prev, uint32_t acc_cost, board_t *board, bool *visited)
{
    // if we're at the end
    if (tuple_eq(here, board->end))
    {
        board->cost[here.r * board->rows + here.c] = acc_cost;
        return acc_cost;
    }

    tuple look_dirs[] = {{-1, 0},
                         {1, 0},
                         {0, -1},
                         {0, 1}};

    tuple moved_dir = tuple_sub(here, prev);
    visited[here.r * board->rows + here.c] = true;
    uint32_t min_fork = UINT32_MAX;
    // look up, down, left and right.
    for (size_t i = 0; i < sizeof(look_dirs) / sizeof(tuple); i++)
    {
        // note: no bounds checking, as the boundary is always a line of '#', so we will never look from edges
        tuple look_pos = tuple_add(look_dirs[i], here);
        char look_char = board->maze[look_pos.r * board->rows + look_pos.c];
        bool is_turn = !tuple_eq(look_dirs[i], moved_dir);
        bool is_reverse = tuple_eq(look_dirs[i], tuple_neg(moved_dir));
        bool look_visited = visited[look_pos.r * board->rows + look_pos.c];

        if (look_char == '#' || is_reverse || look_visited)
        {
            continue;
        }

        uint32_t look_end_cost = board->cost[look_pos.r * board->rows + look_pos.c];
        uint32_t cost_from_here = acc_cost + 1 + (1000 * is_turn);
        if (cost_from_here <= look_end_cost)
        {
            uint32_t fork_depth = dfs_search(look_pos, here, cost_from_here, board, visited);
            if (fork_depth <= board->cost[here.r * board->rows + here.c])
            {
                board->cost[here.r * board->rows + here.c] = fork_depth;
            }

            if (fork_depth <= min_fork)
            {
                min_fork = fork_depth;
            }
        }
    }
    visited[here.r * board->rows + here.c] = false;
    return min_fork;
}

bool is_corner(tuple pos, board_t *board)
{
    char self = board->maze[pos.r * board->rows + pos.c];

    if (self == '#')
    {
        return false;
    }

    tuple look_dirs[] = {{-1, 0},
                         {1, 0},
                         {0, -1},
                         {0, 1}};

    for (size_t i = 0; i < sizeof(look_dirs) / sizeof(tuple); i++)
    {
        tuple look_pos = tuple_add(look_dirs[i], pos);
        char look_char = board->maze[look_pos.r * board->rows + look_pos.c];
        if (look_char == '.') // NOTE: Probably not interesting to also add 'S' or 'E' ?
        {
            return true;
        }
    }

    return false;
}

uint32_t explore_and_connect_edges(node *this, board_t *board, node **node_arr)
{
    tuple look_dirs[] = {{-1, 0},
                         {1, 0},
                         {0, -1},
                         {0, 1}};

    edge *this_edges[] = {&this->up, &this->down, &this->left, &this->right};

    uint32_t edges_count = 0;
    for (size_t i = 0; i < sizeof(look_dirs) / sizeof(tuple); i++)
    {
        tuple look_pos = tuple_add(look_dirs[i], this->pos);
        char look_char = board->maze[look_pos.r * board->rows + look_pos.c];
        size_t j = 0;
        while (look_char != '#')
        {
            node *other = node_arr[look_pos.r * board->rows + look_pos.c];
            edge *other_edges[] = {&this->down, &this->up, &this->right, &this->left};
            if (other != NULL)
            {
                this_edges[i]->node = other;
                this_edges[i]->weight = j;

                other_edges[i]->node = this;
                other_edges[i]->weight = j;

                edges_count++;
            }

            look_pos = tuple_add(tuple_mult(look_dirs[i], (tuple){j, j}), this->pos);
            look_char = board->maze[look_pos.r * board->rows + look_pos.c];
            j++;
        }
    }

    // IDEA: if only one edge found, this is a dead end. This could be pruned
    // down. But it ivolves problems how to recursively do it when the full
    // graph might not have been connected yet.
    return edges_count;
}

node *make_graph(board_t *board)
{
    node *node_arr[board->rows][board->cols];
    for (size_t i = 0; i < board->rows; i++)
    {
        for (size_t j = 0; j < board->cols; j++)
        {
            node_arr[i][j] = NULL;
        }
    }

    node_arr[board->start.r][board->start.c] = node_malloc(board->start);
    node_arr[board->end.r][board->end.c] = node_malloc(board->end);

    // put out nodes
    uint32_t node_count = 0;
    for (size_t i = 0; i < board->rows; i++)
    {
        for (size_t j = 0; j < board->cols; j++)
        {
            tuple ij = {i, j};
            if (is_corner(ij, board))
            {
                node_arr[i][j] = node_malloc(ij);
            }
        }
    }

    // connect nodes
    for (size_t i = 0; i < board->rows; i++)
    {
        for (size_t j = 0; j < board->cols; j++)
        {
            if (node_arr[i][j] == NULL)
            {
                continue;
            }
            node *ij = node_arr[i][j];
            uint32_t edges = explore_and_connect_edges(ij, board, node_arr);
            if (edges == 0)
            {
                free(ij);
            }
        }
    }

    // put all nodes in a smaller array so we can return it. This is because otherwise isolated nodes would be lost.

    return node_arr[board->start.r][board->start.c];
}

int main()
{
    char *inputs = file_reader(INPUTS_PATH);

    size_t rows_n = strcnt(inputs, "\n");
    size_t cols_n = strstr(inputs, "\n") - inputs;

    char *maze = malloc(sizeof(char) * rows_n * cols_n);
    uint32_t *costs = malloc(sizeof(uint32_t) * rows_n * cols_n); // the number is the cost to get there
    bool *visited = malloc(sizeof(bool) * rows_n * cols_n);
    tuple start = {0};
    tuple end = {0};

    // uint32_t *cost_for_best_path_array = malloc(sizeof(uint32_t) * rows_n * cols_n);

    char *line = inputs;
    for (int i = 0; i < rows_n; i++)
    {
        memcpy(maze + (i * cols_n), line, sizeof(char) * cols_n);
        line = strstr(line, "\n") + 1;

        for (int j = 0; j < cols_n; j++)
        {
            costs[i * rows_n + j] = UINT32_MAX;
            visited[i * rows_n + j] = false;
            // cost_for_best_path_array[i * rows_n + j] = UINT32_MAX;

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

    visited[start.r * board.rows + start.c] = true;

    dfs_search(start, (tuple){start.r, start.c - 1}, 0, &board, visited);

    uint32_t cheapest_cost = board.cost[end.r * board.rows + end.c];
    printf("Part 1. Final cost: %u\n", cheapest_cost);

    uint32_t cheapest_path_count = 0;

    for (size_t i = 0; i < board.rows; i++)
    {
        for (size_t j = 0; j < board.cols; j++)
        {
            if (board.cost[i * board.rows + j] == cheapest_cost)
            {
                cheapest_path_count++;
            }
        }
    }

    printf("Part 2. Tiles on best path: %u\n", cheapest_path_count);

    free(inputs);
    inputs = NULL;
}