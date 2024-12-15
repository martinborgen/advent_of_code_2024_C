

/*

https://adventofcode.com/2024/day/14

--- Day 14: Restroom Redoubt ---

One of The Historians needs to use the bathroom; fortunately, you know there's a
bathroom near an unvisited location on their list, and so you're all quickly
teleported directly to the lobby of Easter Bunny Headquarters.

Unfortunately, EBHQ seems to have "improved" bathroom security again after your
last visit. The area outside the bathroom is swarming with robots!

To get The Historian safely to the bathroom, you'll need a way to predict where
the robots will be in the future. Fortunately, they all seem to be moving on the
tile floor in predictable straight lines.

You make a list (your puzzle input) of all of the robots' current positions (p)
and velocities (v), one robot per line. For example:

    p=0,4 v=3,-3
    p=6,3 v=-1,-3
    p=10,3 v=-1,2
    p=2,0 v=2,-1
    p=0,0 v=1,3
    p=3,0 v=-2,-2
    p=7,6 v=-1,-3
    p=3,0 v=-1,-2
    p=9,3 v=2,3
    p=7,3 v=-1,2
    p=2,4 v=2,-3
    p=9,5 v=-3,-3

Each robot's position is given as p=x,y where x represents the number of tiles
the robot is from the left wall and y represents the number of tiles from the
top wall (when viewed from above). So, a position of p=0,0 means the robot is
all the way in the top-left corner.

Each robot's velocity is given as v=x,y where x and y are given in tiles per
second. Positive x means the robot is moving to the right, and positive y means
the robot is moving down. So, a velocity of v=1,-2 means that each second, the
robot moves 1 tile to the right and 2 tiles up.

The robots outside the actual bathroom are in a space which is 101 tiles wide
and 103 tiles tall (when viewed from above). However, in this example, the
robots are in a space which is only 11 tiles wide and 7 tiles tall.

The robots are good at navigating over/under each other (due to a combination of
springs, extendable legs, and quadcopters), so they can share the same tile and
don't interact with each other. Visually, the number of robots on each tile in
this example looks like this:

    1.12.......
    ...........
    ...........
    ......11.11
    1.1........
    .........1.
    .......1...

These robots have a unique feature for maximum bathroom security: they can
teleport. When a robot would run into an edge of the space they're in, they
instead teleport to the other side, effectively wrapping around the edges. Here
is what robot p=2,4 v=2,-3 does for the first few seconds:

Initial state:
    ...........
    ...........
    ...........
    ...........
    ..1........
    ...........
    ...........

After 1 second:
    ...........
    ....1......
    ...........
    ...........
    ...........
    ...........
    ...........

After 2 seconds:
    ...........
    ...........
    ...........
    ...........
    ...........
    ......1....
    ...........

After 3 seconds:
    ...........
    ...........
    ........1..
    ...........
    ...........
    ...........
    ...........

After 4 seconds:
    ...........
    ...........
    ...........
    ...........
    ...........
    ...........
    ..........1

After 5 seconds:
    ...........
    ...........
    ...........
    .1.........
    ...........
    ...........
    ...........

The Historian can't wait much longer, so you don't have to simulate the robots
for very long. Where will the robots be after 100 seconds?

In the above example, the number of robots on each tile after 100 seconds has
elapsed looks like this:

    ......2..1.
    ...........
    1..........
    .11........
    .....1.....
    ...12......
    .1....1....

To determine the safest area, count the number of robots in each quadrant after
100 seconds. Robots that are exactly in the middle (horizontally or vertically)
don't count as being in any quadrant, so the only relevant robots are:

    ..... 2..1.
    ..... .....
    1.... .....

    ..... .....
    ...12 .....
    .1... 1....

In this example, the quadrants contain 1, 3, 4, and 1 robot. Multiplying these
together gives a total safety factor of 12.

Predict the motion of the robots in your list within a space which is 101 tiles
wide and 103 tiles tall. What will the safety factor be after exactly 100
seconds have elapsed?

 */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "file_reader.h"
#include "my_string.h"

#define INPUTS_PATH "../inputs/day14.txt"

#define TILES_WIDTH 101
#define TILES_HEIGHT 103

#define PART2_MAX_T 100000

typedef struct bot
{
    int px, py, vx, vy;
} bot;

bot *read_input(char *inputs_path, size_t *botlist_len)
{
    char *str_in = file_reader(inputs_path);

    size_t lines = strcnt(str_in, "\n");

    bot *botlist = malloc(sizeof(bot) * lines);

    char *token = strtok(str_in, "\n");
    size_t i = 0;
    while (token)
    {
        sscanf(token, "p=%d,%d v=%d,%d", &botlist[i].px, &botlist[i].py, &botlist[i].vx, &botlist[i].vy);

        token = strtok(NULL, "\n");
        i++;
    }

    free(str_in);
    *botlist_len = lines;
    return botlist;
}

// wraps the value around zero and max
int wrap(int a, int max)
{
    return ((a % max) < 0) * max + a % max;
}

void calculate_bot_pos(bot *botlist, size_t botlist_len, int32_t seconds, size_t tiles_w, size_t tiles_h)
{
    for (size_t i = 0; i < botlist_len; i++)
    {
        bot *b = &botlist[i];
        b->px = wrap(b->px + b->vx * seconds, tiles_w);
        b->py = wrap(b->py + b->vy * seconds, tiles_h);
    }
}

void count_quadrants(bot *botlist, size_t botlist_len, size_t tiles_w, size_t tiles_h,
                     int *q1, int *q2, int *q3, int *q4)
{
    // int q1 = 0, q2 = 0, q3 = 0, q4 = 0;
    for (size_t i = 0; i < botlist_len; i++)
    {
        bot *b = &botlist[i];
        if (b->px < tiles_w / 2 && b->py < tiles_h / 2)
        {
            (*q1)++;
        }

        if (b->px > tiles_w / 2 && b->py < tiles_h / 2)
        {
            (*q2)++;
        }

        if (b->px < tiles_w / 2 && b->py > tiles_h / 2)
        {
            (*q3)++;
        }

        if (b->px > tiles_w / 2 && b->py > tiles_h / 2)
        {
            (*q4)++;
        }
    }
}

void print_bots(bot *botlist, size_t botlist_len)
{
    for (size_t i = 0; i < botlist_len; i++)
    {
        printf("p=%d,%d v=%d,%d\n", botlist[i].px, botlist[i].py, botlist[i].vx, botlist[i].vy);
    }
}

void print_board(bot *botlist, size_t botlist_len, size_t tiles_w, size_t tiles_h)
{
    int(*board)[tiles_w] = malloc(sizeof(*board) * tiles_h);

    for (int i = 0; i < tiles_h; i++)
    {
        for (int j = 0; j < tiles_w; j++)
        {
            board[i][j] = 0;
        }
    }

    for (int i = 0; i < botlist_len; i++)
    {
        bot b = botlist[i];
        board[b.py][b.px]++;
    }

    for (int i = 0; i < tiles_h; i++)
    {
        for (int j = 0; j < tiles_w; j++)
        {
            int d = board[i][j];
            if (d > 0)
            {
                printf("%d", d);
            }
            else
            {
                printf(".");
            }
        }
        printf("\n");
    }
    free(board);
    printf("\n");
}

// calculate information entropy
float count_entropy_count(bot *botlist, size_t botlist_len, size_t tiles_w, size_t tiles_h)
{
    float entropy = 0;

    int *bins = calloc(botlist_len, sizeof(int) * botlist_len);

    int(*board)[tiles_w] = malloc(sizeof(*board) * tiles_h);
    for (int i = 0; i < tiles_h; i++)
    {
        for (int j = 0; j < tiles_w; j++)
        {
            board[i][j] = 0;
        }
    }

    for (int i = 0; i < botlist_len; i++)
    {
        bot *b = &botlist[i];
        board[b->py][b->px]++;
    }

    for (int i = 1; i < tiles_h; i++)
    {
        for (int j = 1; j < tiles_w; j++)
        {
            int count = board[i][j];
            bins[count]++;
        }
    }

    int bins_sum = 0;
    for (int i = 0; i < botlist_len; i++)
    {
        bins_sum += bins[i];
    }

    for (int i = 0; i < botlist_len; i++)
    {
        if (bins[i] > 0)
        {
            float prob = (float)bins[i] / (float)bins_sum;
            entropy -= prob * log2f(prob);
        }
    }

    free(board);
    free(bins);
    return entropy;
}

// a different entropy calculation, where the x, y pos are evaluated. A pattern ought to be fairly close.
float count_entropy_pos(bot *botlist, size_t botlist_len, size_t tiles_w, size_t tiles_h)
{
    float entropy_x = 0;
    float entropy_y = 0;

    int *bins_x = calloc(botlist_len, sizeof(int) * botlist_len);
    int *bins_y = calloc(botlist_len, sizeof(int) * botlist_len);

    for (int i = 0; i < botlist_len; i++)
    {
        bot *b = &botlist[i];
        bins_x[b->px]++;
        bins_y[b->py]++;
    }

    int bins_x_sum = 0;
    int bins_y_sum = 0;
    for (int i = 0; i < botlist_len; i++)
    {
        bins_x_sum += bins_x[i];
        bins_y_sum += bins_y[i];
    }

    for (int i = 0; i < botlist_len; i++)
    {
        if (bins_x[i] > 0)
        {
            float prob_x = (float)bins_x[i] / (float)bins_x_sum;
            entropy_x -= prob_x * log2f(prob_x);
        }

        if (bins_y[i] > 0)
        {
            float prob_y = (float)bins_y[i] / (float)bins_y_sum;
            entropy_y -= prob_y * log2f(prob_y);
        }
    }

    free(bins_x);
    free(bins_y);
    return (entropy_x + entropy_y) / 2;
}

// attempt att using variance to find anomaly
int count_variance(bot *botlist, size_t botlist_len, size_t tiles_w, size_t tiles_h)
{
    int q1 = 0, q2 = 0, q3 = 0, q4 = 0;
    count_quadrants(botlist, botlist_len, TILES_WIDTH, TILES_HEIGHT, &q1, &q2, &q3, &q4);

    int average = (q1 + q2 + q3 + q4) / 4;
    int variance = pow(q1 - average, 2) + pow(q2 - average, 2) + pow(q3 - average, 2) + pow(q4 - average, 2);

    return variance;
}

// presumably an anomaly will involve many bots in close proximity
int count_adjecant(bot *botlist, size_t botlist_len, size_t tiles_w, size_t tiles_h)
{
    int(*board)[tiles_w] = malloc(sizeof(*board) * tiles_h);
    int adjecants = 0;
    for (int i = 0; i < tiles_h; i++)
    {
        for (int j = 0; j < tiles_w; j++)
        {
            board[i][j] = 0;
        }
    }

    for (int i = 0; i < botlist_len; i++)
    {
        bot *b = &botlist[i];
        board[b->py][b->px]++;
    }

    for (int i = 1; i < tiles_h; i++)
    {
        for (int j = 1; j < tiles_w; j++)
        {
            if (board[i][j] > 0 && board[i - 1][j] > 0)
            {
                adjecants += board[i - 1][j];
            }

            if (board[i][j] > 0 && board[i][j - 1] > 0)
            {
                adjecants += board[i][j - 1];
            }

            if (board[i][j] > 0 && board[i - 1][j - 1] > 0)
            {
                adjecants += board[i - 1][j - 1];
            }
        }
    }
    free(board);
    return adjecants;
}

void find_anomaly()
{
    uint64_t max_variance = 0;
    uint64_t time_for_max_e = 0;
    float min_entropy = __FLT32_MAX__;

    uint64_t max_adj = 0;
    uint64_t time_max_adj = 0;
    uint64_t time_min_ent = 0;

    size_t botlist_len = 0;
    bot *botlist_original = read_input(INPUTS_PATH, &botlist_len);
    bot *botlist = malloc(sizeof(bot) * botlist_len);

    for (int t = 0; t < PART2_MAX_T; t++)
    {
        memcpy(botlist, botlist_original, botlist_len);

        calculate_bot_pos(botlist, botlist_len, t, TILES_WIDTH, TILES_HEIGHT);

        // int e = count_variance(botlist, botlist_len, TILES_WIDTH, TILES_HEIGHT);
        // if (e > max_variance)
        // {
        //     max_variance = e;
        //     time_for_max_e = t;
        // }

        // int a = count_adjecant(botlist, botlist_len, TILES_WIDTH, TILES_HEIGHT);
        // if (a > max_adj)
        // {
        //     max_adj = a;
        //     time_max_adj = t;
        // }

        float ent = count_entropy(botlist, botlist_len, TILES_WIDTH, TILES_HEIGHT);
        if (ent < min_entropy)
        {
            min_entropy = ent;
            time_min_ent = t;
        }

        // print_board(botlist, botlist_len, TILES_WIDTH, TILES_HEIGHT);
        // printf("\n");
    }

    memcpy(botlist, botlist_original, botlist_len);

    calculate_bot_pos(botlist, botlist_len, time_min_ent, TILES_WIDTH, TILES_HEIGHT);
    print_bots(botlist, botlist_len);
    printf("Min ent is: %f at time %lu\n", min_entropy, time_min_ent);
    print_board(botlist, botlist_len, TILES_WIDTH, TILES_HEIGHT);

    free(botlist);
    free(botlist_original);
}

int main()
{
    size_t botlist_len = 0;
    bot *botlist = read_input(INPUTS_PATH, &botlist_len);

    calculate_bot_pos(botlist, botlist_len, 100, TILES_WIDTH, TILES_HEIGHT);
    print_board(botlist, botlist_len, TILES_WIDTH, TILES_HEIGHT);

    int q1 = 0, q2 = 0, q3 = 0, q4 = 0;
    count_quadrants(botlist, botlist_len, TILES_WIDTH, TILES_HEIGHT, &q1, &q2, &q3, &q4);
    int safety_factor = q1 * q2 * q3 * q4;

    print_bots(botlist, botlist_len);

    printf("\n Safety Factor: %d\n", safety_factor);

    find_anomaly();
    return 0;
}
