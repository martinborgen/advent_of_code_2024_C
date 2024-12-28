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
#include "my_q.h"

#define INPUTS_PATH "../inputs/day17_sample2.txt"

#define THREADS_COUNT 12

struct computer
{
    int64_t regA, regB, regC, PC;
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
        comp->PC++;
        break;

    case 1:
        // The bxl instruction (opcode 1) calculates the bitwise XOR of register B
        // and the instruction's literal operand, then stores the result in register
        // B.

        comp->regB ^= operand;
        comp->PC++;
        break;

    case 2:
        // The bst instruction (opcode 2) calculates the value of its combo operand
        // modulo 8 (thereby keeping only its lowest 3 bits), then writes that value
        // to the B register.

        comp->regB = get_combo(operand, comp) % 8;
        comp->PC++;
        break;

    case 3:
        // The jnz instruction (opcode 3) does nothing if the A register is 0.
        // However, if the A register is not zero, it jumps by setting the
        // instruction pointer to the value of its literal operand; if this
        // instruction jumps, the instruction pointer is not increased by 2
        // after this instruction.

        if (comp->regA == 0)
        {
            comp->PC++;
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
        comp->PC++;
        break;

    case 5:
        // The out instruction (opcode 5) calculates the value of its combo
        // operand modulo 8, then outputs that value. (If a program outputs
        // multiple values, they are separated by commas.)

        comp->PC++;
        return get_combo(operand, comp) % 8;
        break;

    case 6:
        // The bdv instruction (opcode 6) works exactly like the adv instruction
        // except that the result is stored in the B register. (The numerator is
        // still read from the A register.)

        comp->regB = comp->regA / int_pow(2, get_combo(operand, comp));
        comp->PC++;
        break;

    case 7:
        // The cdv instruction (opcode 7) works exactly like the adv instruction
        // except that the result is stored in the C register. (The numerator is
        // still read from the A register.)

        comp->regC = comp->regA / int_pow(2, get_combo(operand, comp));
        comp->PC++;
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

void execute_octal(struct int_vector *program, struct int_vector *output, struct computer *comp)
{
    int32_t *added_hash = malloc(sizeof(int32_t) * program->length);
    for (size_t i = 0; i < program->length; i++)
    {
        added_hash[i] = -1;
    }

    size_t added = 0;
    while (comp->PC >= 0 && comp->PC < program->length)
    {
        int opcode = program->values[comp->PC] / 10;
        int operand = program->values[comp->PC] % 10;
        int this_pc = comp->PC;
        int res = execute(opcode, operand, comp);
        if (res >= 0)
        {
            int_vector_push_back(output, res);
            added++;
        }

        if (added_hash[this_pc] == added)
        {
            // if we've been at this PC and haven't added anything since
            output->length = 0;
            break;
        }

        added_hash[this_pc] = added;
    }
    // return output;
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
        token = strtok(NULL, ",");
        instruction *= 10;
        instruction += atoi(token);
        int_vector_push_back(&program, instruction);
        token = strtok(NULL, ",");
    }
    free(inputs);

    // backtracking with queue
    struct int_vector output = int_vector_new();
    qnode_t *q = NULL;
    size_t no_in_gen = 0; // this is to keep track of 'generations' in q
    size_t no_in_next_gen = 0;

    // first pass to fill queue with some values
    for (int i = 0; i < 0x40; i++)
    {
        struct computer comp = {.regA = i};

        execute_octal(&program, &output, &comp);

        int res = int_vector_to_int10(&output);
        printf("res is %d for i: %d\n", res, i);
        if (res == program.values[program.length - 1])
        {
            q = queue_push(q, i);
            no_in_gen++;
        }

        int_vector_set_capacity(&output, 0);
    }

    // then loop until q is empty
    int32_t octal = program.length - 2;
    while (q != NULL)
    {
        // pop number, calculate target
        int64_t a = queue_pop(&q) << 6;
        int64_t target = 0;
        for (int32_t j = octal; j < program.length; j++)
        {
            target *= 100;
            target += program.values[j];
        }

        for (int i = 0; i < 0x40; i++)
        {
            struct computer comp = {.regA = a + i};

            execute_octal(&program, &output, &comp);

            int64_t res = int_vector_to_int10(&output);
            if (res == target)
            {
                q = queue_push(q, a + i);
                no_in_next_gen++;
                printf("res is %ld for %ld\n", res, a + i);
            }

            int_vector_set_capacity(&output, 0);
        }

        // decrease octal for next iteration if we've just 'exhausted' that position
        no_in_gen--;
        if (no_in_gen == 0)
        {
            octal--;
            no_in_gen = no_in_next_gen;
            no_in_next_gen = 0;
        }

        if (octal < 0)
        {
            break;
        }
    }
    printf("Solution to part 2: %ld\n", queue_pop(&q));
    while (q != NULL)
    {
        queue_pop(&q);
    }
    int_vector_destruct(&program);
    int_vector_destruct(&output);
}