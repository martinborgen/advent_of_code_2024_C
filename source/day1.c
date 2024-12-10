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
#include <stdbool.h>

#define INIT_BUF_SIZE 128
#define READ_LINE_SIZE 128
#define INPUTS_PATH "../inputs/day1.txt"

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

void bubble_sort(int *arr, const size_t arr_size)
{
    if (arr_size < 2)
    {
        return;
    }

    bool is_sorted = false;
    bool has_bubbled;

    while (!is_sorted)
    {
        has_bubbled = false;
        for (int i = 1; i < arr_size; i++)
        {
            if (arr[i - 1] > arr[i])
            {
                int tmp = arr[i];
                arr[i] = arr[i - 1];
                arr[i - 1] = tmp;
                has_bubbled = true;
            }
        }

        if (!has_bubbled)
        {
            is_sorted = true;
        }
    }
}

int binary_search(const int *arr, const size_t arr_len, const int key)
{
    int l = 0;
    int r = arr_len - 1;
    int m;
    int m_val;

    while (l <= r)
    {
        m = l + (r - l) / 2;
        m_val = arr[m];
        if (key == m_val)
        {
            for (int i = m; i >= 0; i--)
            {
                if (arr[i] != key)
                {
                    return i + 1;
                }
            }
            return 0; // edge case of running to beginning of arr
        }
        if (key > m_val)
        {
            l = m + 1;
        }
        else if (l == r && key != arr[l])
        {
            return -1; // key does not exist
        }
        else
        {
            r = m;
        }
    }
    return -1; // unlikely exit point
}

int count_occurances(const int *arr, const size_t arr_len, const int key)
{
    int count = 0;
    int i = binary_search(arr, arr_len, key);
    while (i < arr_len && arr[i] == key)
    {
        count++;
        i++;
    }
    return count;
}

int my_abs(int a)
{
    if (a < 0)
    {
        return -a;
    }
    else
    {
        return a;
    }
}

int main()
{
    // Part 1
    int *nums_left;
    int *nums_right;

    size_t arr_size = read_inputs(INPUTS_PATH, &nums_left, &nums_right, INIT_BUF_SIZE);

    bubble_sort(nums_left, arr_size);
    bubble_sort(nums_right, arr_size);

    int total_distance = 0;

    for (int i = 0; i < arr_size; i++)
    {
        int distance = nums_left[i] - nums_right[i];
        total_distance += my_abs(distance);
    }

    printf("Day 1, part 1. Total distance: %d\n", total_distance);

    // Part 2
    int similarity = 0;

    for (int i = 0; i < arr_size; i++)
    {
        int num = nums_left[i];
        int count = count_occurances(nums_right, arr_size, num);
        similarity += num * count;
    }

    printf("Day 1, part 2. Total similarity: %d\n", similarity);

    free(nums_left);
    free(nums_right);
    return 0;
}