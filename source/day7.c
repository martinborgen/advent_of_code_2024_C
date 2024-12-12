

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

    if (inputs_ptr == NULL)
    {
        printf("Error opening file!\n");
        return -1;
    }

    unsigned long long calibration_result = 0;

    char line_buf[LINE_BUF_SIZE];
    while (fgets(line_buf, LINE_BUF_SIZE, inputs_ptr))
    {
        bool test_val_found = false;
        int spaces = 0;
        struct int_vector nums = int_vector_new();
        char *token = strtok(line_buf, ": ");
        unsigned long long test_val = strtoull(token, NULL, 10);

        while (token != NULL)
        {
            int num = atoi(token);
            int_vector_push_back(&nums, num);
            token = strtok(NULL, " ");
            spaces++;
        }
        spaces--; // the loop always adds one too much

        // permutate operators, from all + to all *
        for (int i = 1; i <= nums.length; i++)
        {
            unsigned long long sum_1 = nums.values[0];
            // first, do product of all initial multiplications
            for (int ii = 1; ii < i; ii++)
            {
                sum_1 *= (unsigned long long)nums.values[ii];
            }

            // then, only sum, except one multiplication which is incremented through the remaining nums
            for (int j = i; j <= nums.length; j++)
            {
                unsigned long long sum_2 = sum_1;
                for (int jj = i; jj < nums.length; jj++)
                {
                    if (jj == j)
                    {
                        sum_2 *= (unsigned long long)nums.values[jj];
                    }
                    else
                    {
                        sum_2 += (unsigned long long)nums.values[jj];
                    }
                }

                if (sum_2 == test_val)
                {
                    calibration_result += test_val;
                    test_val_found = true;
                    break;
                }
            }

            if (test_val_found)
            {
                break;
            }
            else if (sum_1 == test_val)
            {
                calibration_result += test_val;
                test_val_found = true;
                break;
            }
        }
    }

    printf("Part 1. Calibration result: %llu\n", calibration_result);

    fclose(inputs_ptr);
    return 0;
}