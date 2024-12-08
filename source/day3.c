
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define LINE_BUFF_SIZE 8192

// after finding a 'mul(' instance, this function is to determine if it is a
// valid multiplication call
// returns the number of parsed characters
int mul_parser(char *str, size_t str_len, int *dst)
{
    size_t numA_digits = 0;
    size_t numB_digits = 0;

    char numA[4] = "\0";
    char numB[4] = "\0";

    bool rightbracket = false;
    bool comma = false;

    // start is after 'mul(' (4 chars), then two maximum three digit numbers, a comma and final bracket are max to parse
    int i = 4;
    for (i; i < 8; i++)
    {
        char c = str[i];
        if (c <= '9' && c >= '0')
        {
            numA[numA_digits] = c;
            numA_digits++;
        }
        else if (c == ',')
        {
            comma = true;
            break;
        }
        else
        {
            *dst = 0;
            return i;
        }
    }

    for (i++; i < 12; i++)
    {
        char c = str[i];
        if (c <= '9' && c >= '0')
        {
            numB[numB_digits] = c;
            numB_digits++;
        }
        else if (c == ')')
        {
            rightbracket = true;
            break;
        }
        else
        {
            *dst = 0;
            return i;
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
    *dst = A * B;

    return i + 1;
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
    unsigned long output = 0;

    bool mul_enabled = true;

    char mul_key[] = "mul(";
    char do_key[] = "do()";
    char dont_key[] = "don't()";

    size_t i = 0;
    while (i < working_size)
    {
        char c = working[i];
        if (c == 'm' && mul_enabled)
        {
            bool is_mul = true;
            for (int j = 0; j < sizeof(mul_key) - 1; j++)
            {
                if (working[i + j] != mul_key[j])
                {
                    is_mul = false;
                    i += j;
                    break;
                }
            }

            if (is_mul)
            {
                int res;
                int parsed = mul_parser(working + i, working_size - i, &res);
                output += res;
                i += parsed;
            }
        }
        else if (c == 'd')
        {
            // First check for do()
            bool is_do = true;
            int j;
            for (j = 0; j < sizeof(do_key) - 1; j++)
            {
                if (working[i + j] != do_key[j])
                {
                    is_do = false;
                    break;
                }
            }

            if (is_do)
            {
                mul_enabled = true;
            }
            else
            {
                // if not do, then maybe a don't()
                bool is_dont = true;
                for (; j < sizeof(dont_key) - 1; j++)
                {
                    if (working[i + j] != dont_key[j])
                    {
                        is_dont = false;
                    }
                }

                if (is_dont)
                {
                    mul_enabled = false;
                }
            }
            i += j;
        }
        else
        {

            i++;
        }
    }
    printf("Total sum is: %ld\n", output);
}