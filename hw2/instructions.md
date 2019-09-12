# Homework 2
Grading: 30% correctness, 70% style

Due Sept 19 at 9PM

## Description

In this assignment you will implement a simple class hierarchy in multiple files.
You will implement the classes `shape`, (equilateral) `triangle`, and `square`.

Consider questions like the following:
Which classes should be abstract?
Do all of their member functions need to be pure virtual?
What should `const` objects be able to do?

The concrete classes should provide a constructor that takes an `int` for the side length and a default constructor that sets the side length to 0.
They should also provide `void setSideLength(int)` and `double getArea()`.
Don't worry about negative side lengths.

For 10% bonus marks, write `void draw()` to draw ASCII art (sized appropriately) of the shape to `stdout`.
There won't be specific guidelines for the ASCII drawing, as long as it works with different sizes (e.g. you can draw the triangle pointing up, left, right, whatever.).
The triangle also doesn't have to _be_ equilateral (is that possible with ASCII art?), but it should look equilateral (i.e. /\\, not |\\).

The area of an equilateral triangle is `(sqrt(3)/4) * l * l`, where `l` is the side length.

Submit `shape.hpp`, `shape.cpp`, `triangle.hpp`, `triangle.cpp`, `square.hpp`, and `square.cpp`.
We will test using our own `main.cpp`.
Your files should compile without warnings or errors using `g++ -Wall -std=c++17 -g main.cpp shape.cpp triangle.cpp square.cpp`.

The majority of this homework is based on style and design decisions.
If you are not sure about some decision (e.g. const, public/protected/private, whether some common functionality should be moved to the base class, etc.), then write a short comment explaining your reasoning.
