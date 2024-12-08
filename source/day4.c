

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "file_reader.h"

#define INPUTS_PATH "../../inputs/day4.txt"

/* Count instances of `key` in `haystack`. Both `key` and `haystack` must be null-terminated */
int strcnt(const char *haystack, const char *key)
{
    size_t key_len = strlen(key);
    int count = 0;
    char *instance = strstr(haystack, key);
    size_t offset = 0;
    while (instance != NULL)
    {
        count++;
        offset = instance - haystack;
        instance = strstr(haystack + offset + key_len, key);
    }

    return count;
}

/* Copies the row to dst. ´reverse = true` reverses it */
void get_row(char *arr, char *dst, size_t row, size_t rows_n, size_t cols_n, bool reverse)
{
    int idx;
    for (int i = 0; i < cols_n; i++)
    {
        if (reverse)
        {
            idx = cols_n - 1 - i;
        }
        else
        {
            idx = i;
        }
        dst[idx] = arr[row * cols_n + i];
    }
}

/* Copies the col to dst. ´reverse = true` reverses it */
void get_col(char *arr, char *dst, size_t col, size_t rows_n, size_t cols_n, bool reverse)
{
    int idx;
    for (int i = 0; i < rows_n; i++)
    {
        if (reverse)
        {
            idx = rows_n - 1 - i;
        }
        else
        {
            idx = i;
        }
        dst[idx] = arr[i * cols_n + col];
    }
}

int main()
{
    char *puzzle = file_reader(INPUTS_PATH);
    char test[] = "hejhejsvejhejhejsvej";
    int cnt = count_instances(test, "svej");
}