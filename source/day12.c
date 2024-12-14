

/*
https://adventofcode.com/2024/day/12

--- Day 12: Garden Groups ---

Why not search for the Chief Historian near the gardener and his massive farm?
There's plenty of food, so The Historians grab something to eat while they
search.

You're about to settle near a complex arrangement of garden plots when some
Elves ask if you can lend a hand. They'd like to set up fences around each
region of garden plots, but they can't figure out how much fence they need to
order or how much it will cost. They hand you a map (your puzzle input) of the
garden plots.

Each garden plot grows only a single type of plant and is indicated by a single
letter on your map. When multiple garden plots are growing the same type of
plant and are touching (horizontally or vertically), they form a region. For
example:

    AAAA
    BBCD
    BBCC
    EEEC

This 4x4 arrangement includes garden plots growing five different types of
plants (labeled A, B, C, D, and E), each grouped into their own region.

In order to accurately calculate the cost of the fence around a single region,
you need to know that region's area and perimeter.

The area of a region is simply the number of garden plots the region contains.
The above map's type A, B, and C plants are each in a region of area 4. The type
E plants are in a region of area 3; the type D plants are in a region of area 1.

Each garden plot is a square and so has four sides. The perimeter of a region is
the number of sides of garden plots in the region that do not touch another
garden plot in the same region. The type A and C plants are each in a region
with perimeter 10. The type B and E plants are each in a region with perimeter
8. The lone D plot forms its own region with perimeter 4.

Visually indicating the sides of plots in each region that contribute to the
perimeter using - and |, the above map's regions' perimeters are measured as
follows:

    +-+-+-+-+
    |A A A A|
    +-+-+-+-+     +-+
                |D|
    +-+-+   +-+   +-+
    |B B|   |C|
    +   +   + +-+
    |B B|   |C C|
    +-+-+   +-+ +
            |C|
    +-+-+-+   +-+
    |E E E|
    +-+-+-+

Plants of the same type can appear in multiple separate regions, and regions can
even appear within other regions. For example:

    OOOOO
    OXOXO
    OOOOO
    OXOXO
    OOOOO

The above map contains five regions, one containing all of the O garden plots,
and the other four each containing a single X plot.

The four X regions each have area 1 and perimeter 4. The region containing 21
type O plants is more complicated; in addition to its outer edge contributing a
perimeter of 20, its boundary with each X region contributes an additional 4 to
its perimeter, for a total perimeter of 36.

Due to "modern" business practices, the price of fence required for a region is
found by multiplying that region's area by its perimeter. The total price of
fencing all regions on a map is found by adding together the price of fence for
every region on the map.

In the first example, region A has price 4 * 10 = 40, region B has price 4 * 8 =
32, region C has price 4 * 10 = 40, region D has price 1 * 4 = 4, and region E
has price 3 * 8 = 24. So, the total price for the first example is 140.

In the second example, the region with all of the O plants has price 21 * 36 =
756, and each of the four smaller X regions has price 1 * 4 = 4, for a total
price of 772 (756 + 4 + 4 + 4 + 4).

Here's a larger example:

    RRRRIICCFF
    RRRRIICCCF
    VVRRRCCFFF
    VVRCCCJFFF
    VVVVCJJCFE
    VVIVCCJJEE
    VVIIICJJEE
    MIIIIIJJEE
    MIIISIJEEE
    MMMISSJEEE

It contains:

    A region of R plants with price 12 * 18 = 216.
    A region of I plants with price 4 * 8 = 32.
    A region of C plants with price 14 * 28 = 392.
    A region of F plants with price 10 * 18 = 180.
    A region of V plants with price 13 * 20 = 260.
    A region of J plants with price 11 * 20 = 220.
    A region of C plants with price 1 * 4 = 4.
    A region of E plants with price 13 * 18 = 234.
    A region of I plants with price 14 * 22 = 308.
    A region of M plants with price 5 * 12 = 60.
    A region of S plants with price 3 * 8 = 24.

So, it has a total price of 1930.

What is the total price of fencing all regions on your map?

 */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file_reader.h"
#include "my_string.h"

#define INPUTS_PATH "../inputs/day12.txt"

void explore(size_t r, size_t c, size_t rows, size_t cols,
             char board[rows][cols], bool visited[rows][cols],
             uint32_t *area, uint32_t *perimeter)
{
    char current = board[r][c];
    uint32_t own_area = 1;
    uint32_t own_perimeter = 4;
    visited[r][c] = true;

    if (r > 0 && board[r - 1][c] == current)
    {
        own_perimeter--;
        if (visited[r - 1][c] == false)
        {
            explore(r - 1, c, rows, cols, board, visited, area, perimeter);
        }
    }

    if (r < rows - 1 && board[r + 1][c] == current)
    {
        own_perimeter--;
        if (visited[r + 1][c] == false)
        {
            explore(r + 1, c, rows, cols, board, visited, area, perimeter);
        }
    }

    if (c > 0 && board[r][c - 1] == current)
    {
        own_perimeter--;
        if (visited[r][c - 1] == false)
        {
            explore(r, c - 1, rows, cols, board, visited, area, perimeter);
        }
    }

    if (c < cols - 1 && board[r][c + 1] == current)
    {
        own_perimeter--;
        if (visited[r][c + 1] == false)
        {
            explore(r, c + 1, rows, cols, board, visited, area, perimeter);
        }
    }

    *area += own_area;
    *perimeter += own_perimeter;
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

    bool(*visited)[cols_n] = malloc(sizeof(*visited) * rows_n);
    for (int i = 0; i < rows_n; i++)
    {
        for (int j = 0; j < cols_n; j++)
        {
            visited[i][j] = false;
        }
    }

    uint64_t price = 0;
    for (size_t r = 0; r < rows_n; r++)
    {
        for (size_t c = 0; c < cols_n; c++)
        {
            if (visited[r][c])
            {
                continue;
            }

            uint32_t region_area = 0;
            uint32_t region_perimeter = 0;

            explore(r, c, rows_n, cols_n, board, visited, &region_area, &region_perimeter);

            price += region_area * region_perimeter;
            printf("Region %c, with area %u and perimeter %u\n", board[r][c], region_area, region_perimeter);
        }
    }

    printf("Price is: %lu\n", price);
}