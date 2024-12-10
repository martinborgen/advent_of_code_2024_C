

// https://adventofcode.com/2024/day/5

/* The notation X|Y means that if both page number X and page number Y are to be produced as part of an update,
page number X must be printed at some point before page number Y.

For example:

47|53
97|13
97|61
97|47
75|29
61|13
75|53
29|13
97|29
53|29
61|53
97|53
61|29
47|13
75|47
97|75
47|61
75|61
47|29
75|13
53|13

75,47,61,53,29
97,61,53,29,13
75,29,13
75,97,47,61,53
61,13,29
97,13,75,29,47

The first section specifies the page ordering rules, one per line. The first rule, 47|53, means that
if an update includes both page number 47 and page number 53, then page number 47 must be printed at
some point before page number 53. (47 doesn't necessarily need to be immediately before 53; other pages
are allowed to be between them.)

The second section specifies the page numbers of each update. Because most safety manuals are different,
the pages needed in the updates are different too. The first update, 75,47,61,53,29, means that the update
consists of page numbers 75, 47, 61, 53, and 29.



For some reason, the Elves also need to know the middle page number of each update being printed.
Because you are currently only printing the correctly-ordered updates, you will need to find the middle
page number of each correctly-ordered update. In the above example, the correctly-ordered updates are:

75,47,61,53,29
97,61,53,29,13
75,29,13

These have middle page numbers of 61, 53, and 29 respectively. Adding these page numbers together gives 143.

Part 1: Determine which updates are already in the correct order.
What do you get if you add up the middle page number from those correctly-ordered updates?

ADIITIONAL ASSUMPTIONS BY CODE AUTHOR:
    Only two-digit page numbers?
    Page numbers are non-zero?
*/

#include <stdlib.h>
#include <string.h>

#define INPUTS_1_PATH "../../inputs/day5_1.txt"
#define INPUTS_2_PATH "../../inputs/day5_2.txt"

int main()
{
}