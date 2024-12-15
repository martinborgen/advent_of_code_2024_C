/* --- Day 13: Claw Contraption ---

Next up: the lobby of a resort on a tropical island. The Historians take a
moment to admire the hexagonal floor tiles before spreading out.

Fortunately, it looks like the resort has a new arcade! Maybe you can win some
prizes from the claw machines?

The claw machines here are a little unusual. Instead of a joystick or
directional buttons to control the claw, these machines have two buttons labeled
A and B. Worse, you can't just put in a token and play; it costs 3 tokens to
push the A button and 1 token to push the B button.

With a little experimentation, you figure out that each machine's buttons are
configured to move the claw a specific amount to the right (along the X axis)
and a specific amount forward (along the Y axis) each time that button is
pressed.

Each machine contains one prize; to win the prize, the claw must be positioned
exactly above the prize on both the X and Y axes.

You wonder: what is the smallest number of tokens you would have to spend to win
as many prizes as possible? You assemble a list of every machine's button
behavior and prize location (your puzzle input). For example:

Button A: X+94, Y+34 Button B: X+22, Y+67 Prize: X=8400, Y=5400

Button A: X+26, Y+66 Button B: X+67, Y+21 Prize: X=12748, Y=12176

Button A: X+17, Y+86 Button B: X+84, Y+37 Prize: X=7870, Y=6450

Button A: X+69, Y+23 Button B: X+27, Y+71 Prize: X=18641, Y=10279

This list describes the button configuration and prize location of four
different claw machines.

For now, consider just the first claw machine in the list:

    Pushing the machine's A button would move the claw 94 units along the X axis and 34 units along the Y axis.
    Pushing the B button would move the claw 22 units along the X axis and 67 units along the Y axis.
    The prize is located at X=8400, Y=5400; this means that from the claw's initial position, it would need to move exactly 8400 units along the X axis and exactly 5400 units along the Y axis to be perfectly aligned with the prize in this machine.

The cheapest way to win the prize is by pushing the A button 80 times and the B
button 40 times. This would line up the claw along the X axis (because 80*94 +
40*22 = 8400) and along the Y axis (because 80*34 + 40*67 = 5400). Doing this
would cost 80*3 tokens for the A presses and 40*1 for the B presses, a total of
280 tokens.

For the second and fourth claw machines, there is no combination of A and B
presses that will ever win a prize.

For the third claw machine, the cheapest way to win the prize is by pushing the
A button 38 times and the B button 86 times. Doing this would cost a total of
200 tokens.

So, the most prizes you could possibly win is two; the minimum tokens you would
have to spend to win all (two) prizes is 480.

You estimate that each button would need to be pressed no more than 100 times to
win a prize. How else would someone be expected to play?

Figure out how to win as many prizes as possible. What is the fewest tokens you
would have to spend to win all possible prizes?
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "file_reader.h"
#include "my_string.h"
// #include "my_linalg.h"

#define INPUTS_PATH "../inputs/day13.txt"

#define PRICE_A 3
#define PRICE_B 1
#define PART2_CORRECTION 10000000000000
#define EPSILON 0.00000000000001

struct inputs_node
{
    int ax, ay, bx, by, prize_x, prize_y;
    struct inputs_node *next;
};

struct inputs_node *read_inputs(char *inptus_path)
{
    FILE *f = fopen(inptus_path, "r");
    if (f == NULL)
    {
        printf("Error fopen \n");
        return NULL;
    }

    struct inputs_node *start = malloc(sizeof(struct inputs_node));

    const size_t line_size = 1024;
    char line[line_size];
    char *fgets_res;
    struct inputs_node *current = start;
    while (1)
    {
        fgets_res = fgets(line, line_size, f);
        sscanf(line, "Button A: X+%d, Y+%d", &current->ax, &current->ay);
        fgets_res = fgets(line, line_size, f);
        sscanf(line, "Button B: X+%d, Y+%d", &current->bx, &current->by);
        fgets_res = fgets(line, line_size, f);
        sscanf(line, "Prize: X=%d, Y=%d", &current->prize_x, &current->prize_y);
        fgets_res = fgets(line, line_size, f);

        if (fgets_res)
        {
            current->next = malloc(sizeof(struct inputs_node));
            current = current->next;
        }
        else
        {
            break;
        }
    }

    fclose(f);
    return start;
}

uint64_t find_min_price(struct inputs_node *input)
{
    // conveniton here is aij = elements of matrix A, not buttons
    double a11 = input->ax;
    double a12 = input->bx;
    double a21 = input->ay;
    double a22 = input->by;

    double x1 = input->prize_x + PART2_CORRECTION;
    double x2 = input->prize_y + PART2_CORRECTION;

    double b = (a11 * x2 - a21 * x1) / (a11 * a22 - a12 * a21);
    double a = (x1 - a12 * b) / a11;

    double Ad;
    double Bd;
    double a_frac = modf(a, &Ad);
    double b_frac = modf(b, &Bd);

    if (a_frac < EPSILON && b_frac < EPSILON)
    {
        uint64_t A = (uint64_t)(Ad + 0.5);
        uint64_t B = (uint64_t)(Bd + 0.5);
        uint64_t price = A * PRICE_A + B * PRICE_B;
        return price;
    }
    else
    {
        return 0;
    }
}

int main()
{
    uint64_t tokens = 0;
    struct inputs_node *inputs = read_inputs(INPUTS_PATH);
    for (struct inputs_node *current = inputs; current != NULL; current = current->next)
    {
        printf("Button A: X+%d, Y+%d\nButton B: X+%d, Y+%d\nPrize: X=%d, Y=%d\n\n",
               current->ax, current->ay, current->bx, current->by, current->prize_x, current->prize_y);

        uint64_t testres = find_min_price(current);
        printf("%lu\n", testres);

        tokens += testres;
    }

    printf("TOTAL TOKENS: %lu\n", tokens);
    return 0;
}