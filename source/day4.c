

#include <stdio.h>
#include <string.h>
#include "file_reader.h"

#define INPUTS_PATH "../../inputs/day4.txt"

/* Count instances of `key` in `haystack`. Both `key` and `haystack` must be null-terminated */
int count_instances(const char *haystack, const char *key)
{
    size_t key_len = strlen(key);
    int count = 0;
    char *instance = strstr(haystack, key);
    size_t offset = 0;
    while (instance > 0)
    {
        count++;
        offset = instance - haystack;
        instance = strstr(haystack + offset + key_len - 1, key);
    }

    return count;
}

int main()
{
    char *puzzle = file_reader(INPUTS_PATH);
    char test[] = "hejhejsvejhejhejsvej";
    int cnt = count_instances(test, "svej");
}