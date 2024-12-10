

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
#include <stdio.h>
#include <stdbool.h>

#include "vector.h"

#define INPUTS_1_PATH "../inputs/day5_1.txt"
#define INPUTS_2_PATH "../inputs/day5_2.txt"
#define MAX_PAGE_NUMBERS 100
#define LINE_BUFF_SIZE 128

// reads in the rules. max_page_number is used as the rules are hashed, such that `rules[23]` is the rule for number 23
void read_rules(const char *filepath, struct int_vector *rules, const size_t max_page_n)
{
    FILE *inputs_1 = fopen(filepath, "r");

    for (int i = 0; i < max_page_n; i++)
    {
        rules[i] = int_vector_new();
    }

    char line[LINE_BUFF_SIZE];
    while (fgets(line, LINE_BUFF_SIZE, inputs_1))
    {
        int a;
        int b;
        sscanf(line, "%d|%d", &a, &b);

        int_vector_push_back(&rules[a], b);
    }
}

// reads the updates
// takes a pointer to `struct int_vector *updates` as it might be re-alloced if required
// in that case, will also update `size_t n_updates`
void read_updates(const char *filepath, struct int_vector **updates, size_t *n_updates)
{
    FILE *inputs_2 = fopen(filepath, "r");

    if (*n_updates == 0)
    {
        *n_updates = 200;
    }

    *updates = malloc(sizeof(struct int_vector) * *n_updates);

    size_t lines_read = 0;
    char line[LINE_BUFF_SIZE];
    while (fgets(line, LINE_BUFF_SIZE, inputs_2))
    {
        if (lines_read >= *n_updates)
        {
            size_t new_size = *n_updates * 2;
            struct int_vector *tmp = realloc(*updates, sizeof(struct int_vector) * new_size);
            if (tmp == NULL)
            {
                printf("realloc error in read_updates()!\n");
                fclose(inputs_2);
                return;
            }
            else
            {
                *n_updates = new_size;
                *updates = tmp;
            }
        }

        (*updates)[lines_read] = int_vector_new();
        char *tok = strtok(line, ",");
        while (tok != NULL)
        {
            int page_no;
            sscanf(tok, "%d", &page_no);
            int res = int_vector_push_back(&(*updates)[lines_read], page_no);
            if (res != 0)
            {
                printf("Error push_back in read_updates!\n");
                fclose(inputs_2);
                return;
            }
            tok = strtok(NULL, ",");
        }

        lines_read++;
    }

    fclose(inputs_2);
}

void bubblesort_update(struct int_vector *update, struct int_vector *rules)
{
    if (update->length < 2)
    {
        return;
    }

    bool is_sorted = false;
    bool has_bubbled;
    while (!is_sorted)
    {
        has_bubbled = false;
        for (size_t i = 1; i < update->length; i++)
        {
            int current = update->values[i];
            int previous = update->values[i - 1];

            if (int_vector_contains(&rules[current], previous))
            {
                update->values[i - 1] = current;
                update->values[i] = previous;
                has_bubbled = true;
            }
        }

        if (!has_bubbled)
        {
            is_sorted = true;
        }
    }
}

bool verify_update(struct int_vector *update, struct int_vector *rules)
{
    for (size_t i = 1; i <= update->length; i++)
    {
        int current_num = update->values[update->length - i];

        for (size_t j = 0; j < update->length - i; j++)
        {
            int previously_printed = update->values[j];
            if (int_vector_contains(&rules[current_num], previously_printed))
            {
                return false;
            }
        }
    }

    return true;
}

int main()
{
    struct int_vector *rules = malloc(sizeof(struct int_vector) * MAX_PAGE_NUMBERS);
    read_rules(INPUTS_1_PATH, rules, MAX_PAGE_NUMBERS);

    size_t n_updates = 0;
    struct int_vector *updates;
    read_updates(INPUTS_2_PATH, &updates, &n_updates);

    for (int i = 0; i < MAX_PAGE_NUMBERS; i++)
    {
        if (rules[i].length == 0)
        {
            continue;
        }
        printf("Page %d must come before: ", i);
        for (int j = 0; j < rules[i].length; j++)
        {
            printf("%d, ", rules[i].values[j]);
        }
        printf("\n");
    }

    int answer_pt1 = 0;
    int answer_pt2 = 0;
    int failed_sorts = 0;

    printf("The pages read:\n");
    for (int i = 0; i < n_updates; i++)
    {
        for (int j = 0; j < updates[i].length; j++)
        {
            printf("%d, ", updates[i].values[j]);
        }

        if (updates[i].length > 0)
        {
            bool update_is_correct = verify_update(&updates[i], rules);
            size_t mid_index = updates[i].length / 2;
            if (update_is_correct)
            {
                answer_pt1 += updates[i].values[mid_index];
                printf(" - correct!");
            }
            else
            {
                printf(" - incorrect, sorted is:");
                bubblesort_update(&updates[i], rules);

                for (size_t k = 0; k < updates[i].length; k++)
                {
                    printf("%d, ", updates[i].values[k]);
                }

                if (verify_update(&updates[i], rules))
                {
                    printf("Sort sucessful");
                    answer_pt2 += updates[i].values[mid_index];
                }
                else
                {
                    printf("Sort FAILED!");
                    failed_sorts++;
                }
            }
            printf("\n");
        }
    }

    printf("And the answer to part 1 is: %d\n", answer_pt1);

    printf("And the answer to part 2 is: %d\n", answer_pt2);

    printf("NOTE: %d failed sorts\n", failed_sorts);

    return 0;
}