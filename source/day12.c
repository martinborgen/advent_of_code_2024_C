

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

#define INPUTS_PATH "../inputs/day12_sample.txt"

bool is_in_bounds(size_t x, size_t y, size_t xmax, size_t ymax)
{
    if (x >= 0 &&
        x < xmax &&
        y >= 0 &&
        y < ymax)
    {
        return true;
    }
    else
    {
        return false;
    }
}

uint32_t count_inside_corners(size_t r, size_t c, size_t rows, size_t cols,
                              char board[rows][cols])
{
    uint32_t inside_corners = 0;
    char self = board[r][c];

    // directions: left, up-left, up, up-right, right, right-down, down, down-left, and left again
    int x[] = {0, -1, -1, -1, 0, 1, 1, 1, 0};  // look row dir (loops around because reasons)
    int y[] = {-1, -1, 0, 1, 1, 1, 0, -1, -1}; // look col dir (loops around because reasons)

    // loop through directions given by arrays x and y.
    // this loop increments by two, because the arrays x and y
    // also contains the diagonal directions.
    for (int i = 0; i < 8; i += 2)
    {
        // Probably the most hideous if-statement I've ever written.
        // It's just to check that whatever direction we're looking in is in bounds.
        if (r + x[i] >= 0 &&
            c + y[i] >= 0 &&
            r + x[i] < rows &&
            c + y[i] < cols &&
            r + x[i + 1] >= 0 &&
            c + y[i + 1] >= 0 &&
            r + x[i + 1] < rows &&
            c + y[i + 1] < cols &&
            r + x[i + 2] >= 0 &&
            c + y[i + 2] >= 0 &&
            r + x[i + 2] < rows &&
            c + y[i + 2] < cols &&
            board[r + x[i]][c + y[i]] == self &&
            board[r + x[i + 1]][c + y[i + 1]] != self &&
            board[r + x[i + 2]][c + y[i + 2]] == self)
        {
            inside_corners++;
        }
    }
    return inside_corners;
}

uint32_t count_outside_corners(uint64_t r, uint64_t c, size_t rows, size_t cols,
                               char board[rows][cols])
{
    uint32_t outside_corners = 0;
    char self = board[r][c];

    // check if at extreme corners of board
    if ((r == 0 && c == 0) ||
        (r == 0 && c == cols - 1) ||
        (r == rows - 1 && c == 0) ||
        (r == rows - 1 && c == cols - 1))
    {
        outside_corners++;
    }

    // directions: left, up, right, down, and left again
    int x[] = {0, -1, 0, 1, 0};  // look row dir (loops around because reasons)
    int y[] = {-1, 0, 1, 0, -1}; // look col dir (loops around because reasons)

    // loop through directions given by arrays x and y.
    for (int i = 0; i < 4; i++)
    {
        // Probably the second most hideous if-statement I've ever written.
        // It's just to check that whatever direction we're looking in is in bounds.
        if (r + x[i] >= 0 &&
            c + y[i] >= 0 &&
            r + x[i] < rows &&
            c + y[i] < cols &&
            r + x[i + 1] >= 0 &&
            c + y[i + 1] >= 0 &&
            r + x[i + 1] < rows &&
            c + y[i + 1] < cols &&
            board[r + x[i]][c + y[i]] != self &&
            board[r + x[i + 1]][c + y[i + 1]] != self)
        {
            outside_corners++;
        }
        // row or col oob for x[i], y[i], but not for  [i + 1] dir => corner!
        else if ((r + x[i] < 0 || r + x[i] >= rows || c + y[i] < 0 || c + y[i] >= rows) &&
                 r + x[i + 1] >= 0 &&
                 c + y[i + 1] >= 0 &&
                 r + x[i + 1] <= rows - 1 &&
                 c + y[i + 1] <= cols - 1 &&
                 board[r + x[i + 1]][c + y[i + 1]] != self)
        {
            outside_corners++;
        }
        else if ((r + x[i + 1] < 0 || r + x[i + 1] >= rows || c + y[i + 1] < 0 || c + y[i + 1] >= rows) &&
                 r + x[i] >= 0 &&
                 c + y[i] >= 0 &&
                 r + x[i] <= rows - 1 &&
                 c + y[i] <= cols - 1 &&
                 board[r + x[i + 1]][c + y[i + 1]] != self)
        {
            outside_corners++;
        }
    }

    return outside_corners;
}

void explore(size_t r, size_t c, size_t rows, size_t cols,
             char board[rows][cols], bool visited[rows][cols],
             uint32_t *area, uint32_t *inside_corners, uint32_t *outside_corners)
{
    char self = board[r][c];
    uint32_t own_area = 1;
    visited[r][c] = true;

    if (r > 0 && board[r - 1][c] == self)
    {
        if (visited[r - 1][c] == false)
        {
            explore(r - 1, c, rows, cols, board, visited, area, inside_corners, outside_corners);
        }
    }

    if (r < rows - 1 && board[r + 1][c] == self)
    {
        if (visited[r + 1][c] == false)
        {
            explore(r + 1, c, rows, cols, board, visited, area, inside_corners, outside_corners);
        }
    }

    if (c > 0 && board[r][c - 1] == self)
    {
        if (visited[r][c - 1] == false)
        {
            explore(r, c - 1, rows, cols, board, visited, area, inside_corners, outside_corners);
        }
    }

    if (c < cols - 1 && board[r][c + 1] == self)
    {
        if (visited[r][c + 1] == false)
        {
            explore(r, c + 1, rows, cols, board, visited, area, inside_corners, outside_corners);
        }
    }

    *area += own_area;
    *inside_corners += count_inside_corners(r, c, rows, cols, board);
    *outside_corners += count_outside_corners(r, c, rows, cols, board);
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
            uint32_t inside_corners = 0;
            uint32_t outside_corners = 0;

            explore(r, c, rows_n, cols_n, board, visited, &region_area, &inside_corners, &outside_corners);
            uint32_t sides = 4 + 2 * inside_corners; // TODO: Change to use actual outside corners instead of 4
            price += region_area * sides;
            printf("Region %c, with area %u and sides %u\n", board[r][c], region_area, sides);
        }
    }

    printf("Price is: %lu\n", price);
}