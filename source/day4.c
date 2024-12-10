

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "file_reader.h"

#define INPUTS_PATH "../inputs/day4.txt"

/* you're supposed to find two MAS in the shape of an X. One way to achieve that is like this:

M.S
.A.
M.S

 */

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

    for (size_t r = 1; r < rows_n - 1; r++)
    {
        for (size_t c = 1; c < cols_n - 1; c++)
        {
            char chr = array[r][c];
            if (chr == 'A')
            {
                bool x_mas = false;
                if (array[r - 1][c - 1] == 'M' &&
                    array[r + 1][c + 1] == 'S' &&
                    array[r - 1][c + 1] == 'M' &&
                    array[r + 1][c - 1] == 'S')
                {
                    x_mas = true;
                }
                else if (array[r - 1][c - 1] == 'M' &&
                         array[r + 1][c + 1] == 'S' &&
                         array[r - 1][c + 1] == 'S' &&
                         array[r + 1][c - 1] == 'M')
                {
                    x_mas = true;
                }
                else if (array[r - 1][c - 1] == 'S' &&
                         array[r + 1][c + 1] == 'M' &&
                         array[r - 1][c + 1] == 'M' &&
                         array[r + 1][c - 1] == 'S')
                {
                    x_mas = true;
                }
                else if (array[r - 1][c - 1] == 'S' &&
                         array[r + 1][c + 1] == 'M' &&
                         array[r - 1][c + 1] == 'S' &&
                         array[r + 1][c - 1] == 'M')
                {
                    x_mas = true;
                }

                if (x_mas)
                {
                    count++;
                }
            }
        }
    }

    printf("%d", count);
}