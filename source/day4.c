

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

/* writes elements of `arr` into `dst`, depending on starting pos and direction. Returns characters written. */
size_t get_diag(char *dst,
                size_t start_row,
                size_t start_col,
                size_t rows_n,
                size_t cols_n,
                char arr[rows_n][cols_n],
                int x_dir,
                int y_dir)
{
    size_t written = 0;
    size_t step = 0;
    size_t x = start_row;
    size_t y = start_col;

    while (x >= 0 && y >= 0 && x < rows_n && y < cols_n)
    {
        dst[step] = arr[x][y];
        written++;
        step++;
        x += x_dir;
        y += y_dir;
    }

    return written;
}

int main()
{
    char *puzzle = file_reader(INPUTS_PATH);
    size_t rows_n = strcnt(puzzle, "\n");
    size_t cols_n = strstr(puzzle, "\n") - puzzle;

    char(*array)[cols_n] = malloc(sizeof(*array) * rows_n);

    char *row = puzzle;
    for (int i = 0; i < rows_n; i++)
    {
        memcpy(array[i], row, cols_n);
        row = strstr(row, "\n") + 1;
    }
}