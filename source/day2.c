
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define READ_LINE_SIZE 128
#define INIT_BUF_SIZE 128
#define INPUTS_PATH "../inputs/day2.txt"

/*
    Part 1:
    A safe reports have all it's number (levels) either increasing or decreasing.
    Any two adjacent levels differ by at least one and at most three.

    Part 2:
    The Problem Dampener is a reactor-mounted module that lets the reactor safety systems tolerate
    a single bad level in what would otherwise be a safe report. It's like the bad level never happened!
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

// remove_level() removes the number given by `skip` (zero-indexed)
// For instance, `"1 22 33"` with `skip == 1` would return `"1 33"`
void remove_level(char *report, const int skip)
{
    if (skip < 0)
    {
        return;
    }

    int nums_count = 0;
    int last_space_idx = 0;
    int space_idx = 0;
    int len = strnlen(report, READ_LINE_SIZE);

    for (int i = 0; i < len; i++)
    {
        if (report[i] == ' ' || report[i] == '\n')
        {
            nums_count++;
            last_space_idx = space_idx;
            space_idx = i;
        }

        if (nums_count > skip)
        {
            break;
        }
    }

    int shift = space_idx - last_space_idx;
    int i;
    for (i = space_idx; i < len; i++)
    {
        report[i - shift] = report[i];
    }
    report[len - shift] = '\0';
}

int main()
{
    FILE *input_ptr = fopen(INPUTS_PATH, "r"); // Note: make sure this is correct relative to where you run from!

    if (input_ptr == NULL)
    {
        printf("Error opening file!\n");
        return -1;
    }

    char line_buffer[READ_LINE_SIZE];
    int safe_count = 0;

    while (fgets(line_buffer, READ_LINE_SIZE, input_ptr))
    {
        char report_cpy[READ_LINE_SIZE];
        strncpy(report_cpy, line_buffer, READ_LINE_SIZE);
        if (check_report_safety(report_cpy, READ_LINE_SIZE))
        {
            safe_count++;
        }
        else
        {
            // part 2: make permutations of the report with a single level removed
            int levels = 0;
            for (int i = 0; i < strnlen(line_buffer, READ_LINE_SIZE); i++)
            {
                if (line_buffer[i] == ' ')
                {
                    levels++;
                }
            }

            for (int i = 0; i <= levels; i++)
            {
                strncpy(report_cpy, line_buffer, READ_LINE_SIZE);
                remove_level(report_cpy, i);
                if (check_report_safety(report_cpy, READ_LINE_SIZE))
                {

                    safe_count++;
                    break;
                }
            }
        }
    }

    printf("Day 2 part 2: safe count: %d\n", safe_count);
}