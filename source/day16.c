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
    int64_t r, c;
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

typedef struct q_node
{
    node *n;
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

tuple tuple_scalar_mult(tuple a, int b)
{
    return (tuple){a.r * b, a.c * b};
}

int tuple_dot_prod(tuple a, tuple b)
{
    return a.r * b.r + a.c * b.c;
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

void insert_w_prio(q_node *queue, int32_t prio)
{
}

uint32_t dfs_search(node *here, tuple prev_pos, uint32_t acc_cost, board_t *board, bool *visited)
{
    // if we're at the end
    if (tuple_eq(here->pos, board->end))
    {
        board->cost[here->pos.r * board->rows + here->pos.c] = acc_cost;
        return acc_cost;
    }
    printf("at: %lu,%lu\n", here->pos.r, here->pos.c);
    edge *look_edges[] = {&here->up, &here->down, &here->left, &here->right};

    tuple moved_dir = tuple_sub(here->pos, prev_pos);

    visited[here->pos.r * board->rows + here->pos.c] = true;

    uint32_t min_fork = UINT32_MAX;
    uint32_t deadends = 0;
    // look up, down, left and right.
    for (size_t i = 0; i < sizeof(look_edges) / sizeof(edge *); i++)
    {
        // note: no bounds checking, as the boundary is always a line of '#', so we will never look from edges
        node *look_node = look_edges[i]->node;
        if (look_node == NULL)
        {
            deadends++;
            continue;
        }

        tuple look_dir = tuple_sub(look_node->pos, here->pos);
        int32_t dir_dot_prod = tuple_dot_prod(moved_dir, look_dir);
        bool is_turn = dir_dot_prod == 0;
        bool is_reverse = dir_dot_prod < 0;
        bool look_visited = visited[look_node->pos.r * board->rows + look_node->pos.c];

        if (is_reverse || look_visited)
        {
            continue;
        }

        uint32_t look_end_cost = board->cost[look_node->pos.r * board->rows + look_node->pos.c];
        uint32_t cost_from_here = acc_cost + look_edges[i]->weight + (1000 * is_turn);
        if (cost_from_here <= look_end_cost)
        {
            uint32_t fork_depth = dfs_search(look_node, here->pos, cost_from_here, board, visited);
            if (fork_depth <= board->cost[here->pos.r * board->rows + here->pos.c])
            {
                board->cost[here->pos.r * board->rows + here->pos.c] = fork_depth;
            }

            if (fork_depth <= min_fork)
            {
                min_fork = fork_depth;
            }
        }
    }

    // do some cleaning up on the way back from the recursion
    if (deadends == 3)
    {
        if (here->up.node)
        {
            printf("removed node %lu,%lu from %lu,%lu\n", here->pos.r, here->pos.c, here->up.node->pos.r, here->up.node->pos.c);
            here->up.node->down.node = NULL;
        }
        else if (here->down.node)
        {
            printf("removed node %lu,%lu from %lu,%lu\n", here->pos.r, here->pos.c, here->down.node->pos.r, here->down.node->pos.c);
            here->down.node->up.node = NULL;
        }
        else if (here->left.node)
        {
            printf("removed node %lu,%lu from %lu,%lu\n", here->pos.r, here->pos.c, here->left.node->pos.r, here->left.node->pos.c);
            here->left.node->right.node = NULL;
        }
        else if (here->right.node)
        {
            printf("removed node %lu,%lu from %lu,%lu\n", here->pos.r, here->pos.c, here->right.node->pos.r, here->right.node->pos.c);
            here->right.node->left.node = NULL;
        }
    }

    visited[here->pos.r * board->rows + here->pos.c] = false;
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
                         {1, 0}};

    for (size_t i = 0; i < sizeof(look_dirs) / sizeof(tuple); i++)
    {
        tuple look_left = tuple_add((tuple){0, -1}, pos);
        tuple look_right = tuple_add((tuple){0, 1}, pos);
        tuple look_i = tuple_add(look_dirs[i], pos);
        char look_char_i = board->maze[look_i.r * board->rows + look_i.c];
        char look_char_left = board->maze[look_left.r * board->rows + look_left.c];
        char look_char_right = board->maze[look_right.r * board->rows + look_right.c];
        if (look_char_i == '.' && (look_char_left == '.' || look_char_right == '.')) // NOTE: Probably not interesting to also add 'S' or 'E' ?
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
        size_t j = 1;
        while (look_char != '#')
        {
            node *other = node_arr[look_pos.r * board->rows + look_pos.c];
            edge *other_edges[] = {&other->down, &other->up, &other->right, &other->left};
            if (other != NULL)
            {
                this_edges[i]->node = other;
                this_edges[i]->weight = j;

                other_edges[i]->node = this;
                other_edges[i]->weight = j;

                edges_count++;
                break;
            }

            j++;
            look_pos = tuple_add(tuple_scalar_mult(look_dirs[i], j), this->pos);
            look_char = board->maze[look_pos.r * board->rows + look_pos.c];
        }
    }

    // IDEA: if only one edge found, this is a dead end. This could be pruned
    // down. But it ivolves problems how to recursively do it when the full
    // graph might not have been connected yet.
    return edges_count;
}

node **make_graph(board_t *board, size_t *ret_size)
{
    node **node_arr = malloc(sizeof(node *) * board->rows * board->cols);
    for (size_t i = 0; i < board->rows; i++)
    {
        for (size_t j = 0; j < board->cols; j++)
        {
            node_arr[i * board->rows + j] = NULL;
        }
    }

    node_arr[board->start.r * board->rows + board->start.c] = node_malloc(board->start);
    node_arr[board->end.r * board->rows + board->end.c] = node_malloc(board->end);

    // put out nodes
    uint32_t node_count = 0;
    for (size_t i = 0; i < board->rows; i++)
    {
        for (size_t j = 0; j < board->cols; j++)
        {
            tuple ij = {i, j};
            if (is_corner(ij, board))
            {
                node_arr[i * board->rows + j] = node_malloc(ij);
                node_count++;
            }
        }
    }

    // connect nodes
    for (size_t i = 0; i < board->rows; i++)
    {
        for (size_t j = 0; j < board->cols; j++)
        {
            if (node_arr[i * board->rows + j] == NULL)
            {
                continue;
            }
            node *ij = node_arr[i * board->rows + j];
            uint32_t edges = explore_and_connect_edges(ij, board, node_arr);
            if (edges == 0)
            {
                free(ij);
                node_arr[i * board->rows + j] = NULL;
            }
        }
    }

    return node_arr;
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

    size_t graph_node_count = 0;
    node **node_arr = make_graph(&board, &graph_node_count);

    dfs_search(node_arr[start.r * board.rows + start.c], (tuple){start.r, start.c - 1}, 0, &board, visited);

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