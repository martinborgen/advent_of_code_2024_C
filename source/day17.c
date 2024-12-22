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

#include "file_reader.h"
#include "my_string.h"
#include "vector.h"

#define INPUTS_PATH "../inputs/day17.txt"

struct computer
{
    int regA, regB, regC, PC;
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

void execute(int opcode, int operand, struct computer *comp)
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
        printf("%d,", get_combo(operand, comp) % 8);
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

int main()
{
    char *inputs = file_reader(INPUTS_PATH);

    struct computer comp;
    sscanf(inputs, "Register A: %d\nRegister B: %d\nRegister C: %d", &comp.regA, &comp.regB, &comp.regC);
    comp.PC = 0;

    char *program_str = strstr(inputs, "Program: ") + 9;

    struct int_vector program = int_vector_new();

    char *token = strtok(program_str, ",");
    while (token)
    {
        int32_t instruction = atoi(token);
        int_vector_push_back(&program, instruction);
        token = strtok(NULL, ",");
    }

    while (comp.PC >= 0 && comp.PC < program.length - 1)
    {
        int opcode = program.values[comp.PC];
        int operand = program.values[comp.PC + 1];
        execute(opcode, operand, &comp);
    }

    free(inputs);
    return 0;
}