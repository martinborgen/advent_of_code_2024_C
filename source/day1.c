/*  Day 1 of advent of code 2024
    Martin Borgén

    [B]y holding the two lists up side by side (your puzzle input), it quickly becomes clear
    that the lists aren't very similar. Maybe you can help The Historians reconcile their lists?

    For example:

    3   4
    4   3
    2   5
    1   3
    3   9
    3   3

    Maybe the lists are only off by a small amount! To find out, pair up the numbers and measure how far apart they are.
    Pair up the smallest number in the left list with the smallest number in the right list,
    then the second-smallest left number with the second-smallest right number, and so on.

    To find the total distance between the left list and the right list, add up the distances between all of the pairs you found.
    In the example above, this is 2 + 1 + 0 + 1 + 2 + 5, a total distance of 11!

    Your actual left and right lists contain many location IDs. What is the total distance between your lists?
*/

#include <stdlib.h>
#include <stdio.h>

#define INIT_BUF_SIZE 128
#define READ_LINE_SIZE 128

size_t read_inputs(const char *filename, int **dst1, int **dst2, const size_t n)
{
    FILE *fptr;
    int rows_read = 0;
    int arr_size = n;
    char line_buf[READ_LINE_SIZE];

    fptr = fopen(filename, "r");
    *dst1 = malloc(sizeof(int) * INIT_BUF_SIZE);
    *dst2 = malloc(sizeof(int) * INIT_BUF_SIZE);

    if (fptr == NULL)
    {
        printf("Error reading file!\n");
        return -1;
    }
    else if (dst1 == NULL || dst2 == NULL)
    {
        printf("Error malloc returned NULLptrs!\n");
        return -1;
    }

    while (fgets(line_buf, READ_LINE_SIZE, fptr))
    {
        if (rows_read > arr_size)
        {
            int *tmp1 = realloc(*dst1, sizeof(int) * arr_size * 2);
            int *tmp2 = realloc(*dst2, sizeof(int) * arr_size * 2);

            if (tmp1 == NULL || tmp2 == NULL)
            {
                printf("Error realloc returned NULLptrs!\n");
                return -1;
            }
            else
            {
                *dst1 = tmp1;
                *dst2 = tmp2;
                arr_size *= 2;
            }
        }

        int left;
        int right;
        int res = sscanf(line_buf, "%d %d", &left, &right);

        (*dst1)[rows_read] = left;
        (*dst2)[rows_read] = right;

        rows_read++;
    }

    if (arr_size > rows_read)
    {
        int *tmp1 = realloc(*dst1, sizeof(int) * rows_read);
        int *tmp2 = realloc(*dst2, sizeof(int) * rows_read);

        if (tmp1 == NULL || tmp2 == NULL)
        {
            printf("Error realloc returned NULLptrs!\n");
            return -1;
        }
        else
        {
            *dst1 = tmp1;
            *dst2 = tmp2;
            arr_size = rows_read;
        }
    }

    fclose(fptr);
    return arr_size;
}

int main()
{
    printf("Hello world");
}