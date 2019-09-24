# Homework 4
Grading: 50% correctness, 50% style

Since the behavior of this assignment is identical to the last one, correctness is worth less.

Due Oct 3 at 9PM

## Description
In this homework you will redo the [last homework](../hw3/instructions.md) using more idiomatic C++ features. Instead of raw pointers, use `unique_ptr`. Use the copy (or move) and swap idiom for the (copy and move) assignment operator. Write a `swap` function for `node` and use it for the copy and swap idiom.

There should be no raw pointers in your code. Do not use `new` (use `make_unique` instead) or `delete`.

## Submission
Submit `node.hpp` and `node.cpp`.
We will test using our own `main.cpp`.
Your files should compile without warnings or errors using `g++ -Wall -std=c++17 -g main.cpp node.cpp`.
Your executable should never leak memory (test using `valgrind`!).
