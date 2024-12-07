
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define LINE_BUFF_SIZE 8192

int main()
{
    FILE *inputs = fopen("../../inputs/day3.txt", "r");

    if (inputs == NULL)
    {
        printf("Error opening file!\n");
        return -1;
    }

    const size_t working_size = sizeof(char) * (1 << (31 - 1));
    char line_buf[LINE_BUFF_SIZE];
    char *working = malloc(working_size);

    while (fgets(line_buf, LINE_BUFF_SIZE, inputs))
    {
        strncat(working, line_buf, working_size);
    }
    printf("%s", working);
}