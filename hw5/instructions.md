# Homework 5
Grading: 70% correctness, 30% style

Due Oct 26 at 6PM

## Description

In this homework you will write some functions that manipulate containers using iterators.
Only use iterators and the `insert` and `erase` member functions of the containers to implement the functions.
Do not use direct indexing into `vector`s and `deque`s (using `[]`), or the various member functions to insert/delete at the ends of the containers, even though those implementations may be more efficient.
Consult documentation for the different overloads for `insert` and `erase`.
The overload which allows you to pass a start and end iterator may be useful.

Next class we will see ways of reducing the repeated code in writing these functions for different containers, using _templates_.

### `edit`

Write `void edit(vector<int> &)`, `void edit(list<int> &)`, and `void edit(deque<int> &)`.
`edit(c)` should change all occurences of `2` in `c` to two occurences of `2`, and remove all occurences of `0`.
It should not change the rest of `c`.

e.g. (note this is informal syntax, it won't compile):
- `edit({1, 2, 3, 0, 2, 2}) = {1, 2, 2, 3, 2, 2, 2, 2}`
- `edit({}) = {}`
- `edit({2, 2}) = {2, 2, 2, 2}`

### `rotate`

Write `void rotate(vector<int> &, int)`, `void rotate(list<int> &, int)`, and `void rotate(deque<int> &, int)`.
`rotate(c, n)` should perform a circular shift of `c` by a `n` steps.
Note that `n` can be negative.
If `n` is positive, shift to the right, and if `n` is negative shift to the left.

e.g.:
- `rotate({1, 2, 3}, 1) = {3, 1, 2}`
- `rotate({1, 2, 3}, 4) = {3, 1, 2}`
- `rotate({1, 2, 3}, 300001) = {3, 1, 2}`
- `rotate({1, 2, 3}, 0) = {1, 2, 3}`
- `rotate({1, 2, 3}, -1) = {2, 3, 1}`
- `rotate({1, 2, 3}, -2) = {3, 1, 2}`

If you are using `%`, note that the behavior when any of the arguments are negative can be strange.

## Submit instructions

Submit your 6 functions in `functions.hpp`.
We will use our own `main.cpp` to test where we will `#include "functions.hpp"`.
The code should compile without errors or warnings with the options `g++ -Wall -std=c++17 -g main.cpp`.
