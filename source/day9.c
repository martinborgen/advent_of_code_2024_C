/*
https://adventofcode.com/2024/day/9
the disk map (your puzzle input) he's already generated. For example:

2333133121414131402

The disk map uses a dense format to represent the layout of files and free space on the disk.
The digits alternate between indicating the length of a file and the length of free space.

So, a disk map like 12345 would represent a one-block file, two blocks of free space,
a three-block file, four blocks of free space, and then a five-block file.
A disk map like 90909 would represent three nine-block files in a row (with no free space between them).

Each file on disk also has an ID number based on the order of the files as they appear
before they are rearranged, starting with ID 0. So, the disk map 12345 has three files:
a one-block file with ID 0, a three-block file with ID 1, and a five-block file with ID 2.
Using one character for each block where digits are the file ID and . is free space,
the disk map 12345 represents these individual blocks:

0..111....22222

The first example above, 2333133121414131402, represents these individual blocks:

00...111...2...333.44.5555.6666.777.888899

The amphipod would like to move file blocks one at a time from the end of the disk
to the leftmost free space block (until there are no gaps remaining between file blocks).
For the disk map 12345, the process looks like this:

0..111....22222
02.111....2222.
022111....222..
0221112...22...
02211122..2....
022111222......

The first example requires a few more steps:

00...111...2...333.44.5555.6666.777.888899
009..111...2...333.44.5555.6666.777.88889.
0099.111...2...333.44.5555.6666.777.8888..
00998111...2...333.44.5555.6666.777.888...
009981118..2...333.44.5555.6666.777.88....
0099811188.2...333.44.5555.6666.777.8.....
009981118882...333.44.5555.6666.777.......
0099811188827..333.44.5555.6666.77........
00998111888277.333.44.5555.6666.7.........
009981118882777333.44.5555.6666...........
009981118882777333644.5555.666............
00998111888277733364465555.66.............
0099811188827773336446555566..............

The final step of this file-compacting process is to update the filesystem checksum.
To calculate the checksum, add up the result of multiplying each of these blocks'
position with the file ID number it contains. The leftmost block is in position 0.
If a block contains free space, skip it instead.

Continuing the first example, the first few blocks' position multiplied by its file
ID number are 0 * 0 = 0, 1 * 0 = 0, 2 * 9 = 18, 3 * 9 = 27, 4 * 8 = 32, and so on.
In this example, the checksum is the sum of these, 1928.

Compact the amphipod's hard drive using the process he requested.
What is the resulting filesystem checksum? (Be careful copy/pasting the input for this puzzle;
it is a single, very long line.)
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file_reader.h"
#include "vector.h"

#define INPUTS_PATH "../inputs/day9_sample.txt"

// this struct will be used to represent fils.
// Free space is represented using -1 as file ID
typedef struct file_node
{
    int id;
    int blocks;
    struct file_node *next;
    struct file_node *prev;
} file_node;

int main()
{
    char *input = file_reader(INPUTS_PATH);

    int input_len = strlen(input);
    input[input_len - 1] = '\0';
    input_len--;

    file_node deque_begin = {0, (int)input[0] - '0', NULL, NULL};

    file_node *current = &deque_begin;
    for (size_t i = 1; i < input_len - 1; i += 2)
    {
        if (input[i] - '0' > 0)
        {
            // a free space node
            current->next = malloc(sizeof(file_node));
            current->next->prev = current;
            current->next->id = -1;
            current->next->blocks = input[i] - '0';

            current = current->next;
        }

        // a data node
        current->next = malloc(sizeof(file_node));
        current->next->prev = current;
        current->next->id = (i + 1) / 2;
        current->next->blocks = input[i + 1] - '0';

        current = current->next;
    }
    file_node *deque_end = current;
    current->next = NULL; // last datanode

    // print list.
    current = &deque_begin;
    while (current != NULL)
    {
        if (current->id >= 0)
        {
            for (int j = 0; j < current->blocks; j++)
            {
                printf("%d", current->id);
            }
        }
        else
        {
            for (int j = 0; j < current->blocks; j++)
            {
                printf("%c", '.');
            }
        }
        current = current->next;
    }
    printf("\n");
    current = NULL;

    // compacting the drive
    file_node *left = &deque_begin;
    for (; left->id >= 0; left = left->next)
        ;
    file_node *left_start = left;

    file_node *right = deque_end;
    for (; right->id < 0; right = right->prev)
        ;

    // TODO: LOOP TO COMPACT FILE

    printf("\n");

    return 0;
}
