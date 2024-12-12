

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


PART 2:
a third type of operator.

The concatenation operator (||) combines the digits from its left and right inputs into a single number.
For example, 12 || 345 would become 12345. All operators are still evaluated left-to-right.

Now, apart from the three equations that could be made true using only addition and multiplication,
the above example has three more equations that can be made true by inserting operators:

    156: 15 6 can be made true through a single concatenation: 15 || 6 = 156.
    7290: 6 8 6 15 can be made true using 6 * 8 || 6 * 15.
    192: 17 8 14 can be made true using 17 || 8 + 14.

Adding up all six test values (the three that could be made before using only + and * plus the new three
that can now be made by also using ||) produces the new total calibration result of 11387
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

#include "vector.h"

#define INPUTS_PATH "../inputs/day7.txt"
#define LINE_BUF_SIZE 128

uint64_t concat(uint64_t a, uint64_t b)
{
    for (uint64_t b_cpy = b; b_cpy > 0; b_cpy /= 10)
    {
        a *= 10;
    }

    return a + b;
}

bool makes_testval_recursive(uint64_t a, struct int_vector *nums, size_t index, uint64_t testval)
{
    uint64_t val = (uint64_t)nums->values[index];
    if (index == nums->length - 1)
    {
        if (a + val == testval || a * val == testval || concat(a, val) == testval)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    bool add = makes_testval_recursive(a + val, nums, index + 1, testval);
    bool mul = makes_testval_recursive(a * val, nums, index + 1, testval);
    bool cat = makes_testval_recursive(concat(a, val), nums, index + 1, testval);

    return add || mul || cat;
}

int main()
{
    FILE *inputs_ptr = fopen(INPUTS_PATH, "r");

    if (inputs_ptr == NULL)
    {
        printf("Error opening file!\n");
        return -1;
    }

    uint64_t calibration_result = 0;

    char line_buf[LINE_BUF_SIZE];
    while (fgets(line_buf, LINE_BUF_SIZE, inputs_ptr))
    {
        bool test_val_found = false;
        struct int_vector nums = int_vector_new();
        char *token = strtok(line_buf, ": ");
        uint64_t test_val = strtoull(token, &token, 10);
        token = strtok(NULL, " ");

        while (token != NULL)
        {
            int num = atoi(token);
            int_vector_push_back(&nums, num);
            token = strtok(NULL, " ");
        }

        test_val_found = makes_testval_recursive(nums.values[0], &nums, 1, test_val);

        if (test_val_found)
        {
            calibration_result += test_val;
        }

        int_vector_destruct(&nums);
    }

    printf("Calibration result: %lu\n", calibration_result);

    fclose(inputs_ptr);
    return 0;
}