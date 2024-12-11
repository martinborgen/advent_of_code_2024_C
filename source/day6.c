
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

Predict the path of the guard. How many distinct positions will the guard visit before leaving the mapped area?



 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "file_reader.h"
#include "my_string.h"

#define INPUTS_PATH "../inputs/day6.txt"

int main()
{
    char *inputs = file_reader(INPUTS_PATH);

    size_t rows_n = strcnt(inputs, "\n");
    size_t cols_n = strstr(inputs, "\n") - inputs;

    char(*array)[cols_n] = malloc(sizeof(*array) * rows_n);

    char *line = inputs;
    for (int i = 0; i < rows_n; i++)
    {
        memcpy(array[i], line, cols_n);
        line = strstr(line, "\n") + 1;
    }
    free(inputs);
    inputs = NULL;

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
            if (array[r][c] == '^')
            {
                guard_x = r;
                guard_y = c;
            }
        }
    }

    // loop while guard on the board
    while (guard_x > 0 &&
           guard_y > 0 &&
           guard_x < rows_n &&
           guard_y < cols_n)
    {
        bool something_infront;
        if (array[guard_x + guard_dir_x][guard_y + guard_dir_y] == '#')
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

            // Check for visited
            if (array[guard_x][guard_y] == '.')
            {
                array[guard_x][guard_y] = 'X';
                count++;
            }
        }
    }

    printf("Guard visited: %d squares\n", count);
    return 0;
}