
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define LINE_BUFF_SIZE 8192

// after finding a 'mul(' instance, this function is to determine if it is a valid multiplication call
int mul_parser(char *str, size_t str_len)
{
    size_t numA_digits = 0;
    size_t numB_digits = 0;

    char numA[4] = "\0";
    char numB[4] = "\0";

    bool rightbracket = false;
    bool comma = false;

    // start is after 'mul(' (4 chars), then two maximum three digit numbers, a comma and final bracket are max to parse
    for (int i = 4; i < 12; i++)
    {
        char c = str[i];
        if (c == ',' && !rightbracket)
        {
            comma = true;
        }
        else if (c == ')' && comma)
        {
            rightbracket = true;
            break;
        }
        else if (c >= '0' && c <= '9')
        {
            if (!comma)
            {
                numA[numA_digits] = c;
                numA_digits++;
            }
            else
            {
                numB[numB_digits] = c;
                numB_digits++;
            }
        }
        else
        {
            return 0;
        }
    }

    numA[numA_digits] = '\0';
    numB[numB_digits] = '\0';

    if (!rightbracket || !comma || numA_digits == 0 || numB_digits == 0)
    {
        return 0;
    }

    int A = atoi(numA);
    int B = atoi(numB);

    return A * B;
}

int main()
{
    FILE *inputs_ptr = fopen("../../inputs/day3.txt", "r");

    if (inputs_ptr == NULL)
    {
        printf("Error opening file!\n");
        return -1;
    }

    fseek(inputs_ptr, 0, SEEK_END);
    const size_t working_size = ftell(inputs_ptr);
    rewind(inputs_ptr);

    char *working = malloc(working_size + 1);

    fread(working, working_size, 1, inputs_ptr);
    fclose(inputs_ptr);

    working[working_size] = '\0';

    // printf("%s", working);
    int output = 0;
    char key[] = "mul(";
    int i = 0;
    while (i < working_size)
    {
        char *tst = strstr(working + i, key);
        if (tst == NULL)
        {
            break;
        }
        int res = mul_parser(tst, working_size - (int)working - i - (unsigned)tst);
        output += res;
        i += sizeof(key) - 1;
    }
    printf("Total sum is: %d\n", output);
}