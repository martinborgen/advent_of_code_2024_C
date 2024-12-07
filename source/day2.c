
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define READ_LINE_SIZE 128
#define INIT_BUF_SIZE 128

/*
    A safe reports have all it's number (levels) either increasing or decreasing.
    Any two adjacent levels differ by at least one and at most three.
 */
bool check_report_safety(char *report, const size_t report_len)
{
    bool increasing_set = false;
    bool increasing; // false is assumed to be decreasing. Neither increasing or decreasing is not given by this bool!
    int old_level;
    int current_level;
    char *token;

    token = strtok(report, " ");
    old_level = atoi(token);
    token = strtok(NULL, " ");

    while (token != NULL)
    {
        current_level = atoi(token);
        int diff = current_level - old_level;

        if (diff == 0)
        {
            return false;
        }

        if (!increasing_set)
        {
            if (current_level > old_level)
            {
                increasing = true;
            }
            else
            {
                increasing = false;
            }

            increasing_set = true;
        }

        if ((increasing && diff > 3) ||
            (increasing && diff < 0) ||
            (!increasing && diff < -3) ||
            (!increasing && diff > 0))
        {
            return false;
        }

        old_level = current_level;
        token = strtok(NULL, " ");
    }
    return true;
}

int main()
{
    FILE *input_ptr = fopen("../../inputs/day2.txt", "r"); // Note: make sure this is correct relative to where you run from!

    if (input_ptr == NULL)
    {
        printf("Error opening file!\n");
        return -1;
    }

    char line_buffer[READ_LINE_SIZE];
    int safe_count = 0;

    while (fgets(line_buffer, READ_LINE_SIZE, input_ptr))
    {
        // printf("%s", line_buffer);
        if (check_report_safety(line_buffer, READ_LINE_SIZE))
        {
            safe_count++;
        }
    }

    printf("Day 2 part 1: safe count: %d\n", safe_count);
}