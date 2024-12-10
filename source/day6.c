
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

#include "file_reader.h"
#include "my_string.h"

#define INPUTS_PATH "../inputs/day6_sample.txt"

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

    for (size_t i = 0; i < rows_n; i++)
    {
        for (size_t j = 0; j < cols_n; j++)
        {
            printf("%c", array[i][j]);
        }
        printf("\n");
    }
    return 0;
}