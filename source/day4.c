

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
                const size_t start_row,
                const size_t start_col,
                const size_t rows_n,
                const size_t cols_n,
                const char arr[rows_n][cols_n],
                const int x_dir,
                const int y_dir)
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

int my_max(int a, int b)
{
    if (a > b)
    {
        return a;
    }
    else
    {
        return b;
    }
}

int main()
{
    char *puzzle = file_reader(INPUTS_PATH);
    size_t rows_n = strcnt(puzzle, "\n");
    size_t cols_n = strstr(puzzle, "\n") - puzzle;

    char(*array)[cols_n] = malloc(sizeof(*array) * rows_n);

    char *line = puzzle;
    for (int i = 0; i < rows_n; i++)
    {
        memcpy(array[i], line, cols_n);
        line = strstr(line, "\n") + 1;
    }

    char *xmas = "XMAS";
    int count = 0;
    // find all horizontal
    char *row = malloc(sizeof(char) * (cols_n + 1));
    row[cols_n] = '\0';
    for (int i = 0; i < rows_n; i++)
    {
        get_diag(row, i, 0, rows_n, cols_n, array, 0, 1);
        count += strcnt(row, xmas);

        get_diag(row, i, cols_n - 1, rows_n, cols_n, array, 0, -1);
        count += strcnt(row, xmas);
    }
    free(row);

    // find all veritcal
    char *col = malloc(sizeof(char) * (rows_n + 1));
    col[rows_n] = '\0';
    for (int i = 0; i < cols_n; i++)
    {
        get_diag(col, 0, i, rows_n, cols_n, array, 1, 0);
        count += strcnt(col, xmas);

        get_diag(col, rows_n - 1, i, rows_n, cols_n, array, -1, 0);
        count += strcnt(col, xmas);
    }
    free(col);

    // find all diagonal
    size_t diag_return;
    size_t diag_max = my_max(rows_n, cols_n);
    char *diag = malloc(sizeof(char) * (diag_max + 1));
    for (int i = 0; i < rows_n; i++)
    {
        diag_return = get_diag(diag, i, 0, rows_n, cols_n, array, 1, 1);
        diag[diag_return] = '\0';
        count += strcnt(diag, xmas);

        diag_return = get_diag(diag, i, 0, rows_n, cols_n, array, -1, 1);
        diag[diag_return] = '\0';
        count += strcnt(diag, xmas);

        diag_return = get_diag(diag, i, cols_n - 1, rows_n, cols_n, array, -1, -1);
        diag[diag_return] = '\0';
        count += strcnt(diag, xmas);

        diag_return = get_diag(diag, i, cols_n - 1, rows_n, cols_n, array, 1, -1);
        diag[diag_return] = '\0';
        count += strcnt(diag, xmas);
    }

    for (int i = 0; i < cols_n; i++)
    {
        if (i != 0)
        {
            diag_return = get_diag(diag, 0, i, rows_n, cols_n, array, 1, 1);
            diag[diag_return] = '\0';
            count += strcnt(diag, xmas);

            diag_return = get_diag(diag, rows_n - 1, i, rows_n, cols_n, array, -1, 1);
            diag[diag_return] = '\0';
            count += strcnt(diag, xmas);
        }

        if (i != cols_n - 1)
        {
            diag_return = get_diag(diag, 0, i, rows_n, cols_n, array, 1, -1);
            diag[diag_return] = '\0';
            count += strcnt(diag, xmas);

            diag_return = get_diag(diag, rows_n - 1, i, rows_n, cols_n, array, -1, -1);
            diag[diag_return] = '\0';
            count += strcnt(diag, xmas);
        }
    }
    free(diag);

    printf("%d", count);
}