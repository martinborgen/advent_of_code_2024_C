
#include <stdlib.h>
#include <string.h>

#include "my_string.h"

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