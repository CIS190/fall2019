# Homework 3
Grading: 70% correctness, 30% style

Due Sept 26 at 9PM

## Description

In this homework you will implement a binary search tree (BST) using raw pointers.
Complete the implementation of the class `node`, representing a node in the tree.

A BST is a binary tree with the invariant that all the nodes in the left subtree have lower values than the current node, and all nodes in the right subtree have higher values than the current node.

Implement the member functions `bool node::search(int) const` (which returns true iff the argument is in the BST) and `void node::insert(int)`.
This is not meant to be the hard part of the homework.
If you need a hint, consult the [Wikipedia article](https://en.wikipedia.org/wiki/Binary_search_tree) (but don't copy the code!).

Make sure the class doesn't leak any memory using the destructor.
Since you are implementing the destructor, implement the rest of the member functions for the rule of five (copy constructor, move constructor, copy assignment, and move assignment).

## Submission
Submit `node.hpp` and `node.cpp`.
We will test using our own `main.cpp`.
Your files should compile without warnings or errors using `g++ -Wall -std=c++17 -g main.cpp node.cpp`.
Your executable should never leak memory (test using `valgrind`!).
