/*

https://adventofcode.com/2024/day/17#part2

make a computer according to specs.

Part 1: find what the computer outputs.

part 2: find a value of register A such that the computer outputs a compy of the program!

 */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <pthread.h>

#include "file_reader.h"
#include "my_string.h"
#include "vector.h"

#define INPUTS_PATH "../inputs/day17_sample2.txt"

#define THREADS_COUNT 12

struct computer
{
    int64_t regA, regB, regC, PC;
};

struct thread_args
{
    struct int_vector *program;
    size_t range_start, range_end;
    int64_t *retval;
    pthread_mutex_t *retval_mutex;
};

int int_pow(int base, int exp)
{
    int out = 1;
    for (int i = 0; i < exp; i++)
    {
        out *= base;
    }
    return out;
}

int get_combo(int operand, struct computer *comp)
{
    switch (operand)
    {
    case 4:
        return comp->regA;
        break;

    case 5:
        return comp->regB;
        break;

    case 6:
        return comp->regC;
        break;

    default:
        return operand;
        break;
    }
}

int execute(int opcode, int operand, struct computer *comp)
{

    switch (opcode)
    {
    case 0:
        // The adv instruction (opcode 0) performs division. The numerator is the
        // value in the A register. The denominator is found by raising 2 to the
        // power of the instruction's combo operand. (So, an operand of 2 would
        // divide A by 4 (2^2); an operand of 5 would divide A by 2^B.) The result
        // of the division operation is truncated to an integer and then written to
        // the A register.

        comp->regA = comp->regA / int_pow(2, get_combo(operand, comp));
        comp->PC += 2;
        break;

    case 1:
        // The bxl instruction (opcode 1) calculates the bitwise XOR of register B
        // and the instruction's literal operand, then stores the result in register
        // B.

        comp->regB ^= operand;
        comp->PC += 2;
        break;

    case 2:
        // The bst instruction (opcode 2) calculates the value of its combo operand
        // modulo 8 (thereby keeping only its lowest 3 bits), then writes that value
        // to the B register.

        comp->regB = get_combo(operand, comp) % 8;
        comp->PC += 2;
        break;

    case 3:
        // The jnz instruction (opcode 3) does nothing if the A register is 0.
        // However, if the A register is not zero, it jumps by setting the
        // instruction pointer to the value of its literal operand; if this
        // instruction jumps, the instruction pointer is not increased by 2
        // after this instruction.

        if (comp->regA == 0)
        {
            comp->PC += 2;
            break;
        }
        else
        {
            comp->PC = operand;
        }
        break;

    case 4:
        // The bxc instruction (opcode 4) calculates the bitwise XOR of register
        // B and register C, then stores the result in register B. (For legacy
        // reasons, this instruction reads an operand but ignores it.)

        comp->regB = comp->regB ^ comp->regC;
        comp->PC += 2;
        break;

    case 5:
        // The out instruction (opcode 5) calculates the value of its combo
        // operand modulo 8, then outputs that value. (If a program outputs
        // multiple values, they are separated by commas.)

        comp->PC += 2;
        return get_combo(operand, comp) % 8;
        break;

    case 6:
        // The bdv instruction (opcode 6) works exactly like the adv instruction
        // except that the result is stored in the B register. (The numerator is
        // still read from the A register.)

        comp->regB = comp->regA / int_pow(2, get_combo(operand, comp));
        comp->PC += 2;
        break;

    case 7:
        // The cdv instruction (opcode 7) works exactly like the adv instruction
        // except that the result is stored in the C register. (The numerator is
        // still read from the A register.)

        comp->regC = comp->regA / int_pow(2, get_combo(operand, comp));
        comp->PC += 2;
        break;

    default:
        break;
    }

    return -1; // presumably the computer cannot return negative values, so this works as a null-value
}

bool compare_vectors(struct int_vector *a, struct int_vector *b)
{
    if (a->length != b->length)
    {
        return false;
    }

    for (int i = 0; i < a->length; i++)
    {
        if (a->values[i] != b->values[i])
        {
            return false;
        }
    }
    return true;
}

// worker function that will try to find an comp->regA such that output == input within range_start and range_end.
void *find_A_reg(void *args)
{
    // unpacking of the args
    struct thread_args *args_struct = (struct thread_args *)args;
    struct int_vector *program = args_struct->program;
    size_t range_start = args_struct->range_start;
    size_t range_end = args_struct->range_end;
    int64_t *retval_ptr = args_struct->retval;

    struct computer comp;
    struct int_vector output = int_vector_new();
    // each index corresponds to the PC, the value is the ammount of added outputs. -1 means unvisited PC
    int32_t *added_hash = malloc(sizeof(int32_t) * program->length);
    for (size_t i = 0; i < program->length; i++)
    {
        added_hash[i] = -1;
    }

    bool in_out_same;
    int64_t i;
    for (i = range_start; i < range_end; i++)
    {
        if (*retval_ptr < i)
        {
            break;
        }

        comp.PC = 0;
        comp.regA = i;
        comp.regB = 0;
        comp.regC = 0;
        output.length = 0; // setting this effectively clears the vector without resize

        int added = 0;
        // printf("i: %d\n", i);
        while (comp.PC >= 0 && comp.PC < program->length - 1)
        {
            int opcode = program->values[comp.PC];
            int operand = program->values[comp.PC + 1];
            int this_pc = comp.PC;
            int res = execute(opcode, operand, &comp);
            if (res >= 0)
            {
                int_vector_push_back(&output, res);
                added++;
            }

            if ((output.length > 0 && output.values[output.length - 1] != program->values[output.length - 1]) ||
                (added_hash[this_pc] == added))
            {
                // if the recent output is a mismatch w. what we're looking for,
                // or if we've been at this PC and haven't added anything since
                // printf("break due to loop\n");
                break;
            }

            added_hash[this_pc] = added;
        }

        in_out_same = compare_vectors(program, &output);
        if (in_out_same)
        {
            break;
        }

        for (size_t j = 0; j < program->length; j++)
        {
            added_hash[j] = -1;
        }
    }

    if (in_out_same)
    {
        printf("Part 2: ans is %ld\n", i);
        pthread_mutex_lock(args_struct->retval_mutex);
        *retval_ptr = (i < *retval_ptr) ? i : *retval_ptr;
        pthread_mutex_unlock(args_struct->retval_mutex);
    }
    else
    {
        printf("Part 2: solution not in range tested\n");
    }
    return 0;
}

int main()
{
    char *inputs = file_reader(INPUTS_PATH);

    char *program_str = strstr(inputs, "Program: ") + 9;

    struct int_vector program = int_vector_new();

    char *token = strtok(program_str, ",");
    while (token)
    {
        int32_t instruction = atoi(token);
        int_vector_push_back(&program, instruction);
        token = strtok(NULL, ",");
    }
    free(inputs);

    pthread_mutex_t res_mutex;
    pthread_mutex_init(&res_mutex, NULL);
    int64_t res = INT64_MAX;

    pthread_t my1, my2;
    struct thread_args arg1 = {&program,
                               0,
                               INT32_MAX / 2,
                               &res,
                               &res_mutex};
    struct thread_args arg2 = {&program,
                               INT32_MAX / 2 + 1,
                               INT32_MAX,
                               &res,
                               &res_mutex};

    pthread_create(&my1, NULL, find_A_reg, &arg1);
    pthread_create(&my2, NULL, find_A_reg, &arg2);

    pthread_join(my1, NULL);
    pthread_join(my2, NULL);

    printf("res is %ld", res);
}