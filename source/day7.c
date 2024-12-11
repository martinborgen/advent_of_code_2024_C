

/* determine which test values could possibly be produced by placing any combination of operators
into their calibration equations (your puzzle input).

For example:

190: 10 19
3267: 81 40 27
83: 17 5
156: 15 6
7290: 6 8 6 15
161011: 16 10 13
192: 17 8 14
21037: 9 7 18 13
292: 11 6 16 20

Operators are always evaluated left-to-right, not according to precedence rules.
There are two different types of operators: add (+) and multiply (*).


need the total calibration result, which is the sum of the test values from just the equations that could possibly be true.
In the above example, the sum of the test values for the three equations listed above is 3749.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#include "vector.h"

#define INPUTS_PATH "../inputs/day7_sample.txt"
#define LINE_BUF_SIZE 128

int main()
{
    FILE *inputs_ptr = fopen(INPUTS_PATH, "r");

    char line_buf[LINE_BUF_SIZE];
    while (fgets(line_buf, LINE_BUF_SIZE, inputs_ptr))
    {
        char token[LINE_BUF_SIZE];
        strtok(line_buf, ":");
        int test_val = atoi(token);
        while (token != NULL)
        {
            strtok(NULL, " ");
        }
    }

    fclose(inputs_ptr);
}