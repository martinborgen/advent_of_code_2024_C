
/*

https://adventofcode.com/2024/day/6

aybe you can work out where the guard will go ahead of time so that The Historians can search safely?

You start by making a map (your puzzle input) of the situation. For example:

....#.....
.........#
..........
..#.......
.......#..
..........
.#..^.....
........#.
#.........
......#...

The map shows the current position of the guard with ^ (to indicate the guard is currently facing up from the perspective of the map). Any obstructions - crates, desks, alchemical reactors, etc. - are shown as #.

Lab guards in 1518 follow a very strict patrol protocol which involves repeatedly following these steps:

    If there is something directly in front of you, turn right 90 degrees.
    Otherwise, take a step forward.

Part 1: Predict the path of the guard. How many distinct positions will the guard visit before leaving the mapped area?

Part 2: Add one more obstruction to the board such that the guard finds himself in a loop. How many such positions are there?

 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "file_reader.h"
#include "my_string.h"

#define INPUTS_PATH "../inputs/day6.txt"

void print_board(const size_t rows_n, const size_t cols_n, char board[rows_n][cols_n])
{
    for (size_t r = 0; r < rows_n; r++)
    {
        for (size_t c = 0; c < cols_n; c++)
        {
            printf("%c", board[r][c]);
        }
        printf("\n");
    }
    printf("\n");
}

/* Calculates how many discinct positions the guard moves to. Returns -1 if the guard is in a loop
Will modify board to show the path taken. */
int count_guard_positions(const size_t rows_n, const size_t cols_n, char board[rows_n][cols_n])
{
    /*  Board is represented as an array
        ---> y
        |
        V
        x
    */
    int count = 1;
    int guard_x;
    int guard_y;
    int guard_dir_x = -1;
    int guard_dir_y = 0;

    // find start pos
    for (size_t r = 0; r < rows_n; r++)
    {
        for (size_t c = 0; c < cols_n; c++)
        {
            if (board[r][c] == '^')
            {
                guard_x = r;
                guard_y = c;
            }
        }
    }

    // loop while guard on the board and not going in a loop
    while (1)
    {
        bool something_infront;

        // plenty of bounds checking, last check is the actually interesting one
        if (guard_x + guard_dir_x < rows_n &&
            guard_x + guard_dir_x >= 0 &&
            guard_y + guard_dir_y < cols_n &&
            guard_y + guard_dir_y >= 0 &&
            board[guard_x + guard_dir_x][guard_y + guard_dir_y] == '#')
        {
            something_infront = true;
        }
        else
        {
            something_infront = false;
        }

        if (something_infront)
        {
            // turn 90 degrees clockwise.
            if (guard_dir_x > 0)
            {
                guard_dir_x = 0;
                guard_dir_y = -1;
            }
            else if (guard_dir_x < 0)
            {
                guard_dir_x = 0;
                guard_dir_y = 1;
            }
            else if (guard_dir_y > 0)
            {
                guard_dir_x = 1;
                guard_dir_y = 0;
            }
            else
            {
                guard_dir_x = -1;
                guard_dir_y = 0;
            }
        }
        else
        {
            // moving forwards
            guard_x += guard_dir_x;
            guard_y += guard_dir_y;

            char current_square = board[guard_x][guard_y];
            // Check again (!) to avoid index oob on the board
            if (guard_x < 0 ||
                guard_y < 0 ||
                guard_x >= rows_n ||
                guard_y >= cols_n)
            {
                // means moving out fo the board.
                return count;
            }
            else if (current_square == '.')
            {
                // square is unvisited, mark it, count it!
                if (guard_dir_x > 0)
                {
                    board[guard_x][guard_y] = 'd';
                }
                else if (guard_dir_x < 0)
                {
                    board[guard_x][guard_y] = 'u';
                }
                else if (guard_dir_y < 0)
                {
                    board[guard_x][guard_y] = 'l';
                }
                else if (guard_dir_y > 0)
                {
                    board[guard_x][guard_y] = 'r';
                }
                count++;
            }
            else if (((current_square == 'u' || current_square == '^') && guard_dir_x == -1) ||
                     (current_square == 'd' && guard_dir_x == 1) ||
                     (current_square == 'l' && guard_dir_y == -1) ||
                     (current_square == 'r' && guard_dir_y == 1))
            {
                // means loop
                return -1;
            }
        }
    }

    return count;
}

int main()
{
    char *inputs = file_reader(INPUTS_PATH);

    size_t rows_n = strcnt(inputs, "\n");
    size_t cols_n = strstr(inputs, "\n") - inputs;

    char(*board)[cols_n] = malloc(sizeof(*board) * rows_n);

    char *line = inputs;
    for (int i = 0; i < rows_n; i++)
    {
        memcpy(board[i], line, cols_n);
        line = strstr(line, "\n") + 1;
    }
    free(inputs);
    inputs = NULL;

    size_t start_x;
    size_t start_y;
    // find start pos
    for (size_t r = 0; r < rows_n; r++)
    {
        for (size_t c = 0; c < cols_n; c++)
        {
            if (board[r][c] == '^')
            {
                start_x = r;
                start_y = c;
            }
        }
    }

    char(*board_copy)[cols_n] = malloc(sizeof(*board) * rows_n); // board copy, becuse count_guard_positions modifies board
    memcpy(board_copy, board, rows_n * cols_n);
    int count_part1 = count_guard_positions(rows_n, cols_n, board_copy);

    printf("Part 1. Guard visited: %d squares\n", count_part1);

    int loop_count = 0;
    for (size_t r = 0; r < rows_n; r++)
    {
        for (size_t c = 0; c < rows_n; c++)
        {
            memcpy(board_copy, board, rows_n * cols_n);
            if (board_copy[r][c] != '.' ||
                board_copy[r][c] == '^')
            {
                continue;
            }

            board_copy[r][c] = '#';
            int res = count_guard_positions(rows_n, cols_n, board_copy);
            if (res < 0)
            {
                loop_count++;
            }
        }
    }

    printf("Part 2. Number of possible loop counts found: %d\n", loop_count);

    return 0;
}