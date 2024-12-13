

/*

https://adventofcode.com/2024/day/11

The ancient civilization on Pluto was known for its ability to manipulate
spacetime, and while The Historians explore their infinite corridors, you've
noticed a strange set of physics-defying stones.

At first glance, they seem like normal stones: they're arranged in a perfectly
straight line, and each stone has a number engraved on it.

The strange part is that every time you blink, the stones change.

Sometimes, the number engraved on a stone changes. Other times, a stone might
split in two, causing all the other stones to shift over a bit to make room in
their perfectly straight line.

As you observe them for a while, you find that the stones have a consistent
behavior. Every time you blink, the stones each simultaneously change according
to the first applicable rule in this list:

    If the stone is engraved with the number 0, it is replaced by a stone engraved with the number 1.

    If the stone is engraved with a number that has an even number of digits, it is replaced by two stones.
    The left half of the digits are engraved on the new left stone, and the right half of the digits are engraved on the new right stone.
    (The new numbers don't keep extra leading zeroes: 1000 would become stones 10 and 0.)

    If none of the other rules apply, the stone is replaced by a new stone; the old stone's number multiplied by 2024 is engraved on the new stone.

No matter how the stones change, their order is preserved, and they stay on
their perfectly straight line.

How will the stones evolve if you keep blinking at them? You take a note of the
number engraved on each stone in the line (your puzzle input).

If you have an arrangement of five stones engraved with the numbers 0 1 10 99
999 and you blink once, the stones transform as follows:

    The first stone, 0, becomes a stone marked 1.
    The second stone, 1, is multiplied by 2024 to become 2024.
    The third stone, 10, is split into a stone marked 1 followed by a stone marked 0.
    The fourth stone, 99, is split into two stones marked 9.
    The fifth stone, 999, is replaced by a stone marked 2021976.

So, after blinking once, your five stones would become an arrangement of seven
stones engraved with the numbers 1 2024 1 0 9 9 2021976.

Here is a longer example:

Initial arrangement: 125 17

After 1 blink: 253000 1 7

After 2 blinks: 253 0 2024 14168

After 3 blinks: 512072 1 20 24 28676032

After 4 blinks: 512 72 2024 2 0 2 4 2867 6032

After 5 blinks: 1036288 7 2 20 24 4048 1 4048 8096 28 67 60 32

After 6 blinks: 2097446912 14168 4048 2 0 2 4 40 48 2024 40 48 80 96 2 8 6 7 6 0
3 2

In this example, after blinking six times, you would have 22 stones. After
blinking 25 times, you would have 55312 stones!

Consider the arrangement of stones in front of you. How many stones will you
have after blinking 25 times?

*/

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "file_reader.h"

#define INPUTS_PATH "../inputs/day11.txt"
#define MAXIT 45
#define PRE_HASHED 10

typedef struct num_node
{
    uint64_t num;
    struct num_node *next;
} num_node;

uint64_t int_pow(uint64_t a, uint64_t x)
{
    uint64_t out = 1;
    for (uint64_t i = 0; i < x; i++)
    {
        out *= a;
    }
    return out;
}

bool split_digits(uint64_t n, uint64_t *a, uint64_t *b)
{
    bool retval = false;
    int digits = 0;
    for (uint64_t x = n; x > 0; x /= 10)
    {
        digits++;
    }

    if (digits % 2 == 0)
    {
        retval = true;
        int split = int_pow(10, digits / 2);
        *a = n / split;
        *b = n % split;
    }
    else
    {
        retval = false;
    }

    return retval;
}

uint64_t update(uint64_t num, uint32_t iterations, uint32_t maxit, uint64_t hash[PRE_HASHED][MAXIT])
{
    uint64_t output = 0;
    uint64_t digits = (uint64_t)(log10(num) + 1);

    // recursive end
    if (iterations == maxit - 1)
    {
        if (num > 0 && digits % 2 == 0)
        {
            // printf("%lu, returning 2\n", num);
            return 2;
        }
        else
        {
            // printf("%lu, returning 1\n", num);
            return 1;
        }
    }
    else if (num < PRE_HASHED && hash != NULL)
    {
        return hash[num][iterations];
    }

    // recursive

    if (num == 0)
    {
        output += update(1, iterations + 1, maxit, hash);
    }
    else if (digits % 2 == 0)
    {
        uint64_t a, b;
        split_digits(num, &a, &b);
        output += update(a, iterations + 1, maxit, hash);
        output += update(b, iterations + 1, maxit, hash);
    }
    else
    {
        output += update(num * 2024, iterations + 1, maxit, hash);
    }
    return output;
}

int main()
{
    char *input = file_reader(INPUTS_PATH);

    num_node start = {0, NULL};

    char *token;
    token = strtok(input, " ");
    start.num = atoi(token);
    token = strtok(NULL, " ");
    num_node *current = &start;
    while (token != NULL)
    {
        current->next = malloc(sizeof(num_node));
        current = current->next;
        current->num = atoi(token);
        token = strtok(NULL, " ");
    }
    current->next = NULL;

    uint64_t hash[PRE_HASHED][MAXIT];
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < MAXIT; j++)
        {
            hash[i][j] = update(i, j, MAXIT, NULL);
        }
    }

    current = &start;
    uint64_t count = 0;
    while (current != NULL)
    {
        if (current->num < PRE_HASHED)
        {
            count += hash[current->num][0];
        }
        else
        {
            count += update(current->num, 0, MAXIT, hash);
        }
        current = current->next;
    }

    printf("%lu\n", count);
}