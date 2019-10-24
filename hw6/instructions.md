# Homework 6
Grading: 70% correctness, 30% style

Due Oct 31 at 6PM

## Description

There are two parts to this homework.

### `edit`

In the last homework, you wrote `edit` specialized to `vector<int>`, `list<int>`, and `deque<int>`.
Rewrite `edit` from [homework 5](../hw5/instructions.md) as a function template in `edit.hpp`.
The template argument should abstract over the argument type, so `edit` should now have the declaration `template <typename T> void edit(T &)`.

### `unique_pointer`

Write your own `unique_pointer` class template, a simplified version of `unique_ptr`.
Implement the member functions declared in [`unique_pointer.hpp`](unique_pointer.hpp).
You can implement them in the header file or write your own implementation file and `#include` it in the header.

The class member functions should behave the same way as the standard `unique_ptr` (see the [reference](https://en.cppreference.com/w/cpp/memory/unique_ptr))
If you need to throw an exception, throwing anything (like an `int` or `string`) is fine.

## Submission instructions

Submit `edit.hpp` and `unique_pointer.hpp`, and implementation file(s) if you chose to separate the interface and implementation of your templates.

In our `main1.cpp` files we will `#include "edit.hpp"`, and in `main2.cpp` we will `#include "unique_pointer.hpp"`.

The code should compile without errors or warnings with the options `g++ -Wall -std=c++17 -g main1.cpp` (and for `main2.cpp`).
Your executables should never leak memory (test using `valgrind`!).
