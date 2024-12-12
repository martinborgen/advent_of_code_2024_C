

/*
https://adventofcode.com/2024/day/8

Scanning across the city, you find that there are actually many such antennas.
 Each antenna is tuned to a specific frequency indicated by a single lowercase
 etter, uppercase letter, or digit. You create a map (your puzzle input) of these
 antennas. For example:

............
........0...
.....0......
.......0....
....0.......
......A.....
............
............
........A...
.........A..
............
............

The signal only applies its nefarious effect at specific antinodes based on the
resonant frequencies of the antennas. In particular, an antinode occurs at any
point that is perfectly in line with two antennas of the same frequency - but
only when one of the antennas is twice as far away as the other. This means
that for any pair of antennas with the same frequency, there are two antinodes,
one on either side of them.

So, for these two antennas with frequency a, they create the two antinodes
marked with #:

..........
...#......
..........
....a.....
..........
.....a....
..........
......#...
..........
..........

Adding a third antenna with the same frequency creates several more
antinodes. It would ideally add four antinodes, but two are off the right
side of the map, so instead it adds only two:

..........
...#......
#.........
....a.....
........a.
.....a....
..#.......
......#...
..........
..........

Antennas with different frequencies don't create antinodes;
A and a count as different frequencies. However, antinodes can occur at
locations that contain antennas. In this diagram, the lone antenna with
frequency capital A creates no antinodes but has a lowercase-a-frequency
antinode at its location:

..........
...#......
#.........
....a.....
........a.
.....a....
..#.......
......A...
..........
..........

The first example has antennas with two different frequencies,
so the antinodes they create look like this, plus an antinode
overlapping the topmost A-frequency antenna:

......#....#
...#....0...
....#0....#.
..#....0....
....0....#..
.#....A.....
...#........
#......#....
........A...
.........A..
..........#.
..........#.

Because the topmost A-frequency antenna overlaps with
a 0-frequency antinode, there are 14 total unique locations
that contain an antinode within the bounds of the map.

Calculate the impact of the signal. How many unique locations
within the bounds of the map contain an antinode?
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file_reader.h"
#include "vector.h"
#include "my_string.h"

#define INPUTS_PATH "../inputs/day8.txt"

int max_int(int a, int b)
{
    if (a > b)
    {
        return a;
    }
    else
    {
        return b;
    }
}

int min_int(int a, int b)
{
    if (a < b)
    {
        return a;
    }
    else
    {
        return b;
    }
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

    bool(*is_node)[cols_n] = malloc(sizeof(*is_node) * rows_n);
    for (size_t i = 0; i < rows_n; i++)
    {
        for (size_t j = 0; j < cols_n; j++)
        {
            is_node[i][j] = false;
        }
    }

    unsigned node_count = 0;
    for (size_t r = 0; r < rows_n; r++)
    {
        for (size_t c = 0; c < cols_n; c++)
        {
            if (board[r][c] == '.')
            {
                continue;
            }

            // now looping through all squares again, to see if we find a matching frequency.
            for (int i = 0; i < rows_n; i++)
            {
                for (int j = 0; j < cols_n; j++)
                {
                    if (i == r && j == c && board[i][j] != '.')
                    {
                        continue;
                    }
                    else if (board[i][j] == board[r][c])
                    {
                        // if so, step from the nodes by direction * step, in both
                        // positive and negative direction.

                        // positive
                        int delta_row = i - r;
                        int delta_col = j - c;

                        int node_r = i;
                        int node_c = j;
                        int step = 0;
                        while (node_r >= 0 && node_r < rows_n &&
                               node_c >= 0 && node_c < cols_n)
                        {
                            if (is_node[node_r][node_c] == false)
                            {
                                is_node[node_r][node_c] = true;
                                node_count++;
                            }
                            node_r = r + delta_row * step;
                            node_c = c + delta_col * step;
                            step++;
                        }

                        // negative
                        node_r = i;
                        node_c = j;
                        step = 0;
                        while (node_r >= 0 && node_r < rows_n &&
                               node_c >= 0 && node_c < cols_n)
                        {
                            if (is_node[node_r][node_c] == false)
                            {
                                is_node[node_r][node_c] = true;
                                node_count++;
                            }
                            node_r = r + delta_row * step;
                            node_c = c + delta_col * step;
                            step--;
                        }
                    }
                }
            }
        }
    }

    printf("%u\n", node_count);

    free(board);
    return 0;
}