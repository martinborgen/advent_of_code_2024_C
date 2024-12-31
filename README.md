# This is advent of code 2024

https://adventofcode.com/2024

A new programming challenge each day from 1st to 25th December. 
I decided to write it in C

## A little log of the problems
A few notes about each problem. 

### Day 1
I did bubble sort, as it's easy to implement. The inputs weren't very long, and since I'm writing in C, I can afford the performance hit. 

I did a binary search for the similarity score, in order to find where each number was stored in the sorted list. A hashed set or similar would have been a better solution, but also more work to implement. 

### Day 2:
Mostly a permutation problem, nothing particular of note

### Day 3:
Kinda finicky parser to implement, as the input is a continuous string. 
For par 1, I used strtok to find each instance of the mul -keyword, but for part 2 when other words also had to be checked I reverted to a simple linear search. 

### Day 4:
I did a get_diag() function to extract all directions from the string. However in part 2, my solution became more naïve, just checking for the letter 'A', as it would form the centre of an 'X-MAS'. 

### Day 5:
For this problem, I implemented my own int_vector -struct, with assorted function in its own header, as I figured it would be useful in later problems. 

### Day 6:
I did a brute-force solution for part 2. A more efficient approach would have been to only consider squares next to the path you identify in part one. 

### Day 7:
A recursive solution, nothing fancy

### Day 8:
mostly using 2D-vector algebra. Otherwise, four levels of nesting are kind ugly, but it's not a very long function. 

### Day 9:
Solved using a doulby linked list, and some manipulations of it to solve the defragmentation problem

### Day 10:
count paths on a board. Nothing remarkable

### Day 11:
One of the more fun ones! Part one worked with my kind-of-naive solution, but the time-complexity of part 2 shot that down hard. I had to bit the bullet and implement a hash-table for ints, which did the trick!

### Day 12:
It took a fun while to figure ot the math for this one. In the end, I arrived at counting inside and outside corners to get the number of sides. 

### Day 13:
At first I tried doing this with integers only, definingn my own quota-stucts. However, even using i64 the numerators and nominators would grow too large and overflow, so in the end I had to use floats, which probev quite trivial. 

### Day 14:
By far the most memorable of these problems. The initial robot position calculation was quite trivial, as they move with constant speed and diretion. The second part however, when asked to identify a pattern among thousands of combination, and no further hints was a lot of fun. Unfortunately, I had a bug in how I copied the bot-input list for each iteration, which meant no pattern would ever emerge, but more on that later. The upshot of that was that I came up with several methods (all working, it would turn out), before I found the bug. 

My first idea was to use the quadrants we had to use in part 1, and compute their variance. A pattern ought to show a larger variance between them. Using variance instead of standard deviation was easier, as no square-root was necessary. 

My second idea was to cound the number of bots adjecant to each other. A pattern ought to, by its very nature, have an unusually large ammount of bots right next to each other.

My third idea was to compute the entropy of the bots positions. Entropy, as in statistical learning, means a less ordered (more evenly distributed random) bot distribution across the board. It was when this method showed consistent entropy for all iterations I started to suspect I had a bug somewhere else. 

In the end, all methods proved to identify the pattern once the bug was fixed. 

### Day 15:
Kind of a fun little game. Part 1 I had the boxes move recursively. Part 2 made it necessary to first recursively check each box if they all could move, before then recursively move them if possible. 

### Day 16:
A maze solver! Finding the shortest path wasn't too much trouble, I did a depth first recursive solver. However, it tended to miss some parts when part 2 asked for all possible paths. Took a while to figure out why. I also switched from going through each position, to creating a graph with nodes and edges. This is also where some tuple struct and assorted 2D linear algebra functions appeared, which I later moved to their own header. 

### Day 17:
This was fun, simulating a dorky little computer. I implemented all the instructions using switch cases, and the computer state, with registers and program counter as its own struct. For part 1 it computed the output easily. Part 2 was far trickier, it became apparent that any brute-force solution was doomed to fail. I tried implementing a multi-threaded solution in order to really put the 'force' in 'brute force', but I guess when the problem has an exponential time-complexity, multiplying the core-count by a measly scalar like 12 is a grain of sand in the desert. 

In the end, while my CPU was chugging along at 100% usage on all cores, I took some pen and paper, and figured out how the problem could be approached 'backwards', to limit the possible numbers to be investigated. This turned out to solve the problem almost instantly. 

### Day 18:
Another maze-solving algorithm. I tried re-using some from day 16, but it became apparent this problem required a more sophisticated algorithm. A* seemed like a sensible choice, by the use of a minimum priority queue, it was implemented. 


/Martin Borgén 2024
