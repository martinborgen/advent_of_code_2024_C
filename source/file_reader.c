
#include <stdio.h>
#include <stdlib.h>

#include "file_reader.h"

char *file_reader(const char *filename)
{
    FILE *file_ptr = fopen(filename, "r");

    if (file_ptr == NULL)
    {
        printf("Error opening file!\n");
        return NULL;
    }

    fseek(file_ptr, 0, SEEK_END);
    const size_t file_length = ftell(file_ptr);
    rewind(file_ptr);

    char *output = malloc(file_length + 1);

    if (output == NULL)
    {
        printf("Malloc error!\n");
        return NULL;
    }

    fread(output, file_length, 1, file_ptr);
    fclose(file_ptr);

    return output;
}