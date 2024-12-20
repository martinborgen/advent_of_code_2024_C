/*

https://adventofcode.com/2024/day/15

You appear back inside your own mini submarine! Each Historian drives their mini
submarine in a different direction; maybe the Chief has his own submarine down
here somewhere as well?

You look up to see a vast school of lanternfish swimming past you. On closer
inspection, they seem quite anxious, so you drive your mini submarine over to
see if you can help.

Because lanternfish populations grow rapidly, they need a lot of food, and that
food needs to be stored somewhere. That's why these lanternfish have built
elaborate warehouse complexes operated by robots!

These lanternfish seem so anxious because they have lost control of the robot
that operates one of their most important warehouses! It is currently running
amok, pushing around boxes in the warehouse with no regard for lanternfish
logistics or lanternfish inventory management strategies.

Right now, none of the lanternfish are brave enough to swim up to an
unpredictable robot so they could shut it off. However, if you could anticipate
the robot's movements, maybe they could find a safe option.

The lanternfish already have a map of the warehouse and a list of movements the
robot will attempt to make (your puzzle input). The problem is that the
movements will sometimes fail as boxes are shifted around, making the actual
movements of the robot difficult to predict.

    For example:

    ##########
    #..O..O.O#
    #......O.#
    #.OO..O.O#
    #..O@..O.#
    #O#..O...#
    #O..O..O.#
    #.OO.O.OO#
    #....O...#
    ##########

    <vv>^<v^>v>^vv^v>v<>v^v<v<^vv<<<^><<><>>v<vvv<>^v^>^<<<><<v<<<v^vv^v>^
    vvv<<^>^v^^><<>>><>^<<><^vv^^<>vvv<>><^^v>^>vv<>v<<<<v<^v>^<^^>>>^<v<v
    ><>vv>v^v^<>><>>>><^^>vv>v<^^^>>v^v^<^^>v^^>v^<^v>v<>>v^v^<v>v^^<^^vv<
    <<v<^>>^^^^>>>v^<>vvv^><v<<<>^^^vv^<vvv>^>v<^^^^v<>^>vvvv><>>v^<<^^^^^
    ^><^><>>><>^^<<^^v>>><^<v>^<vv>>v>>>^v><>^v><<<<v>>v<v<v>vvv>^<><<>^><
    ^>><>^v<><^vvv<^^<><v<<<<<><^v<<<><<<^^<v<^^^><^>>^<v^><<<^>>^v<v^v<v^
    >^>>^v>vv>^<<^v<>><<><<v<<v><>v<^vv<<<>^^v^>^^>>><<^v>>v^v><^^>>^<>vv^
    <><^^>^^^<><vvvvv^v<v<<>^v<v>v<<^><<><<><<<^^<<<^<<>><<><^^^>^^<>^>v<>
    ^^>vv<^v^v<vv>^<><v<^v>^^^>>>^^vvv^>vvv<>>>^<^>>>>>^<<^v>^vvv<>^<><<v>
    v^^>>><<^^<>>^v^<v^vv<>v^<<>^<^v^v><^<<<><<^<v><v<>vv>>v><v^<vv<>v^<<^

As the robot (@) attempts to move, if there are any boxes (O) in the way, the
robot will also attempt to push those boxes. However, if this action would cause
the robot or a box to move into a wall (#), nothing moves instead, including the
robot. The initial positions of these are shown on the map at the top of the
document the lanternfish gave you.

The rest of the document describes the moves (^ for up, v for down, < for left,
> for right) that the robot will attempt to make, in order. (The moves form a
single giant sequence; they are broken into multiple lines just to make
copy-pasting easier. Newlines within the move sequence should be ignored.)

Here is a smaller example to get started:

    ########
    #..O.O.#
    ##@.O..#
    #...O..#
    #.#.O..#
    #...O..#
    #......#
    ########

    <^^>>>vv<v>>v<<

Were the robot to attempt the given sequence of moves, it would push around the
boxes as follows:

    Initial state:
    ########
    #..O.O.#
    ##@.O..#
    #...O..#
    #.#.O..#
    #...O..#
    #......#
    ########

    Move <:
    ########
    #..O.O.#
    ##@.O..#
    #...O..#
    #.#.O..#
    #...O..#
    #......#
    ########

    Move ^:
    ########
    #.@O.O.#
    ##..O..#
    #...O..#
    #.#.O..#
    #...O..#
    #......#
    ########

    Move ^:
    ########
    #.@O.O.#
    ##..O..#
    #...O..#
    #.#.O..#
    #...O..#
    #......#
    ########

    Move >:
    ########
    #..@OO.#
    ##..O..#
    #...O..#
    #.#.O..#
    #...O..#
    #......#
    ########

    Move >:
    ########
    #...@OO#
    ##..O..#
    #...O..#
    #.#.O..#
    #...O..#
    #......#
    ########

    Move >:
    ########
    #...@OO#
    ##..O..#
    #...O..#
    #.#.O..#
    #...O..#
    #......#
    ########

    Move v:
    ########
    #....OO#
    ##..@..#
    #...O..#
    #.#.O..#
    #...O..#
    #...O..#
    ########

    Move v:
    ########
    #....OO#
    ##..@..#
    #...O..#
    #.#.O..#
    #...O..#
    #...O..#
    ########

    Move <:
    ########
    #....OO#
    ##.@...#
    #...O..#
    #.#.O..#
    #...O..#
    #...O..#
    ########

    Move v:
    ########
    #....OO#
    ##.....#
    #..@O..#
    #.#.O..#
    #...O..#
    #...O..#
    ########

    Move >:
    ########
    #....OO#
    ##.....#
    #...@O.#
    #.#.O..#
    #...O..#
    #...O..#
    ########

    Move >:
    ########
    #....OO#
    ##.....#
    #....@O#
    #.#.O..#
    #...O..#
    #...O..#
    ########

    Move v:
    ########
    #....OO#
    ##.....#
    #.....O#
    #.#.O@.#
    #...O..#
    #...O..#
    ########

    Move <:
    ########
    #....OO#
    ##.....#
    #.....O#
    #.#O@..#
    #...O..#
    #...O..#
    ########

    Move <:
    ########
    #....OO#
    ##.....#
    #.....O#
    #.#O@..#
    #...O..#
    #...O..#
    ########

The larger example has many more moves; after the robot has finished those
moves, the warehouse would look like this:

    ##########
    #.O.O.OOO#
    #........#
    #OO......#
    #OO@.....#
    #O#.....O#
    #O.....OO#
    #O.....OO#
    #OO....OO#
    ##########

The lanternfish use their own custom Goods Positioning System (GPS for short) to
track the locations of the boxes. The GPS coordinate of a box is equal to 100
times its distance from the top edge of the map plus its distance from the left
edge of the map. (This process does not stop at wall tiles; measure all the way
to the edges of the map.)

So, the box shown below has a distance of 1 from the top edge of the map and 4
from the left edge of the map, resulting in a GPS coordinate of 100 * 1 + 4 =
104.

    #######
    #...O..
    #......

The lanternfish would like to know the sum of all boxes' GPS coordinates after
the robot finishes moving. In the larger example, the sum of all boxes' GPS
coordinates is 10092. In the smaller example, the sum is 2028.

Predict the motion of the robot and boxes in the warehouse. After the robot is
finished moving, what is the sum of all boxes' GPS coordinates?

PART 2 The lanternfish use your information to find a safe moment to swim in and
turn off the malfunctioning robot! Just as they start preparing a festival in
your honor, reports start coming in that a second warehouse's robot is also
malfunctioning.

This warehouse's layout is surprisingly similar to the one you just helped.
There is one key difference: everything except the robot is twice as wide! The
robot's list of movements doesn't change.

To get the wider warehouse's map, start with your original map and, for each
tile, make the following changes:

    If the tile is #, the new map contains ## instead.
    If the tile is O, the new map contains [] instead.
    If the tile is ., the new map contains .. instead.
    If the tile is @, the new map contains @. instead.

This will produce a new warehouse map which is twice as wide and with wide boxes
that are represented by []. (The robot does not change size.)

The larger example from before would now look like this:

####################
##....[]....[]..[]##
##............[]..##
##..[][]....[]..[]##
##....[]@.....[]..##
##[]##....[]......##
##[]....[]....[]..##
##..[][]..[]..[][]##
##........[]......##
####################

Because boxes are now twice as wide but the robot is still the same size and
speed, boxes can be aligned such that they directly push two other boxes at
once.
*/

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file_reader.h"
#include "my_string.h"

#define INPUTS0_PATH "../inputs/day15_0.txt"
#define INPUTS1_PATH "../inputs/day15_1.txt"

void print_board(size_t board_rows, size_t board_cols, char board[board_rows][board_cols]);

bool can_move(size_t r, size_t c,
              int r_dir, int c_dir,
              size_t board_rows, size_t board_cols, char board[board_rows][board_cols])
{
    char ahead = board[r + r_dir][c + c_dir];
    bool ans = false;

    if (ahead == '.')
    {
        ans = true;
    }
    else if (ahead == 'O' ||
             (r_dir == 0 && (ahead == '[' || ahead == ']')))
    {
        ans = can_move(r + r_dir, c + c_dir, r_dir, c_dir, board_rows, board_cols, board);
    }
    else if (ahead == '[' && r_dir != 0)
    {
        bool other = can_move(r + r_dir, c + c_dir + 1, r_dir, c_dir, board_rows, board_cols, board);
        ans = other && can_move(r + r_dir, c + c_dir, r_dir, c_dir, board_rows, board_cols, board);
    }
    else if (ahead == ']' && r_dir != 0)
    {
        bool other = can_move(r + r_dir, c + c_dir - 1, r_dir, c_dir, board_rows, board_cols, board);
        ans = other && can_move(r + r_dir, c + c_dir, r_dir, c_dir, board_rows, board_cols, board);
    }
    return ans;
}

// moves a piece, and those ahead on the board. Does not check if possible. use `can_move()` for that
void move(size_t r, size_t c,
          int r_dir, int c_dir,
          size_t board_rows, size_t board_cols, char board[board_rows][board_cols])
{

    char ahead = board[r + r_dir][c + c_dir];
    if (ahead == 'O' ||
        (r_dir == 0 && (ahead == '[' || ahead == ']')))
    {

        move(r + r_dir, c + c_dir, r_dir, c_dir, board_rows, board_cols, board);
    }
    else if (ahead == '[' && r_dir != 0)
    {

        move(r + r_dir, c + c_dir, r_dir, c_dir, board_rows, board_cols, board);
        move(r + r_dir, c + c_dir + 1, r_dir, c_dir, board_rows, board_cols, board);
    }
    else if (ahead == ']' && r_dir != 0)
    {

        move(r + r_dir, c + c_dir, r_dir, c_dir, board_rows, board_cols, board);
        move(r + r_dir, c + c_dir - 1, r_dir, c_dir, board_rows, board_cols, board);
    }

    board[r + r_dir][c + c_dir] = board[r][c];
    board[r][c] = '.';
}

void process_movements(char *movements, size_t board_rows, size_t board_cols,
                       char board[board_rows][board_cols])
{
    // find start
    size_t bot_r, bot_c;
    for (size_t r = 0; r < board_rows; r++)
    {
        for (size_t c = 0; c < board_cols; c++)
        {
            if (board[r][c] == '@')
            {
                bot_r = r;
                bot_c = c;
            }
        }
    }

    // process moves
    for (char *cp = movements; *cp != '\0'; cp++)
    {
        // print_board(board_rows, board_cols, board);
        char c;
        switch (c = *cp)
        {
        case '^':
            if (can_move(bot_r, bot_c, -1, 0, board_rows, board_cols, board))
            {
                move(bot_r, bot_c, -1, 0, board_rows, board_cols, board);
                bot_r--;
            }
            break;

        case '<':
            if (can_move(bot_r, bot_c, 0, -1, board_rows, board_cols, board))
            {
                move(bot_r, bot_c, 0, -1, board_rows, board_cols, board);
                bot_c--;
            }
            break;

        case '>':
            if (can_move(bot_r, bot_c, 0, 1, board_rows, board_cols, board))
            {
                move(bot_r, bot_c, 0, 1, board_rows, board_cols, board);
                bot_c++;
            }
            break;

        case 'v':
            if (can_move(bot_r, bot_c, 1, 0, board_rows, board_cols, board))
            {
                move(bot_r, bot_c, 1, 0, board_rows, board_cols, board);
                bot_r++;
            }
            break;

        default:
            break;
        }
    }
}

void print_board(size_t board_rows, size_t board_cols, char board[board_rows][board_cols])
{
    for (size_t r = 0; r < board_rows; r++)
    {
        for (size_t c = 0; c < board_cols; c++)
        {
            printf("%c", board[r][c]);
        }
        printf("\n");
    }
    printf("\n");
}

int compute_coordinate_sum(size_t board_rows, size_t board_cols, char board[board_rows][board_cols])
{
    int sum = 0;
    for (size_t r = 0; r < board_rows; r++)
    {
        for (size_t c = 0; c < board_cols; c++)
        {
            if (board[r][c] == 'O' || board[r][c] == '[')
            {
                sum += 100 * r + c;
            }
        }
    }
    return sum;
}

// doubles the board with according to the spec.
// NOTE! board_cols must be board 1 cols, not board 2!
void double_board_width(size_t board_rows, size_t board_cols,
                        char board[board_rows][board_cols],
                        char board2[board_rows][2 * board_cols])
{
    for (size_t r = 0; r < board_rows; r++)
    {
        for (size_t c = 0; c < board_cols; c++)
        {
            char tile = board[r][c];
            if (tile == '@')
            {
                board2[r][2 * c] = '@';
                board2[r][(2 * c) + 1] = '.';
            }
            else if (tile == 'O')
            {
                board2[r][2 * c] = '[';
                board2[r][(2 * c) + 1] = ']';
            }
            else
            {
                board2[r][2 * c] = tile;
                board2[r][(2 * c) + 1] = tile;
            }
        }
    }
}

int main()
{
    char *inputs0 = file_reader(INPUTS0_PATH);

    size_t rows_n = strcnt(inputs0, "\n");
    size_t cols_n = strstr(inputs0, "\n") - inputs0;

    char(*board)[cols_n] = malloc(sizeof(*board) * rows_n);

    char *line = inputs0;
    for (int i = 0; i < rows_n; i++)
    {
        memcpy(board[i], line, cols_n);
        line = strstr(line, "\n") + 1;
    }
    free(inputs0);
    inputs0 = NULL;
    size_t rows2_n = rows_n;
    size_t cols2_n = 2 * cols_n;
    char(*board2)[cols2_n] = malloc(sizeof(*board2) * rows2_n);
    double_board_width(rows_n, cols_n, board, board2);

    char *movement = file_reader(INPUTS1_PATH);
    process_movements(movement, rows_n, cols_n, board);

    print_board(rows_n, cols_n, board);

    int coord_sum = compute_coordinate_sum(rows_n, cols_n, board);

    printf("Par 1. Coordinate sum: %d\n", coord_sum);

    print_board(rows2_n, cols2_n, board2);
    process_movements(movement, rows2_n, cols2_n, board2);

    int coord_sum2 = compute_coordinate_sum(rows2_n, cols2_n, board2);
    printf("Part 2. Coordinate sum: %d\n", coord_sum2);
    free(movement);
    return 0;
}
