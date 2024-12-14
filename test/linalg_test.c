

#include <stdio.h>
#include <stdbool.h>

#include "my_linalg.h"

int main()
{
    int output = 0;

    quota test01 = {6, 3};
    quota test02 = {4, 2};
    quota test03 = {5, 3};
    quota test04 = {1, 2};

    quota res_add1 = quota_add(test01, test02);
    if (res_add1.numerator != 24 || res_add1.denominator != 6)
    {
        printf("Error quota add\n");
    }

    quota res_add2 = quota_add(test03, test04);
    if (res_add2.numerator != 13 || res_add1.denominator != 6)
    {
        printf("Error quota add\n");
    }

    bool res_isint1 = quota_is_int(res_add1);
    if (!res_isint1)
    {
        printf("Error expected isint\n");
    }

    bool res_isint2 = quota_is_int(res_add2);
    if (res_isint2)
    {
        printf("Error expected isint\n");
    }

    quota res_mult1 = quota_mult(test01, test02);
    if (res_mult1.numerator != 24 || res_mult1.denominator != 6)
    {
        printf("error quota mult\n");
    }

    quota res_mult2 = quota_mult(test03, test04);
    if (res_mult2.numerator != 5 || res_mult2.denominator != 6)
    {
        printf("error quota mult\n");
    }

    return output;
}