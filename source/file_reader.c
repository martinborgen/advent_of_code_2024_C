
#include <stdio.h>

#include "file_reader.h"

char *file_reader(const char *filename)
{
    FILE *inputs_ptr = fopen(filename, "r");

    if (inputs_ptr == NULL)
    {
        printf("Error opening file!\n");
        return -1;
    }

    fseek(inputs_ptr, 0, SEEK_END);
    const size_t working_size = ftell(inputs_ptr);
    rewind(inputs_ptr);

    char *output = malloc(working_size + 1);

    if (output == NULL)
    {
        printf("Malloc error!\n");
        return -1;
    }

    fread(output, working_size, 1, inputs_ptr);
    fclose(inputs_ptr);

    return output;
}