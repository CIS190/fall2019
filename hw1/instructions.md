# Homework 1
Grading: 70% correctness, 30% style (e.g. const, good use of abstractions, etc.)

Due Sept 13 at 6PM

If you want to output messages to the user (e.g. "enter 0 or 1", "try again", etc.) please do so using `cerr`. We will ignore output to `stderr` and only check the output of `stdout`.

## Description

You will implement a complex number class, implemented as two `double`s. You will implement `operator+`, `operator+=`, `operator>>`, and `operator<<`.

Think about whether these overloaded functions can be member functions, and what their types should be (const? references?) (hint: none of them should return void).

The addition operators will add complex numbers only, not (`complex` + `int`) for instance.  Note that adding complex numbers is just adding their real and imaginary components pointwise.

The `>>` and `<<` operators will expect input and output to be in the format `{r: 0i: 0}` where the `0`s are numbers that can be read into a double, and the spaces can be any whitespace (but there will always be _some_ whitespace). Whitespace between the first `double` and `i` is also acceptable (this is meant to keep the implementation simple), so `{r: 0 i: 0}` is also valid as input, but you should not print with this whitespace. If a stream tries to read a `complex` and the format is incorrect, it should be set to a failure state (as in [`overload.cpp`](../lecture2/overload.cpp)) (and you can set the state of the `complex` to anything, for simplicity).

The program should do the following:
- Prompt the user to enter 0 to read from stdin or 1 to read from a file.
  - Assume only integers will be entered, and eventually a 0 or 1 will be entered.
  -	If an integer besides 0 and 1 is entered, continue to prompt for input until 0 or 1 is entered.
- If reading from a file, the user will then enter the filename.
  - If the file cannot be opened, the program ends (without error). Nothing should be printed.
- From the selected stream, continually read complex numbers until an ill-formed one is found, or EOF is encountered. After each successful read, print the sum of that complex number and `{r: 1i: 1}` (1 + 1i).
  - Each printed complex number should be separated by a newline.
- Before the program exits, the sum of all complex numbers successfully read should be printed, followed by a newline.

Submit [`hw1.cpp`](hw1.cpp), which has been started for you. You should not add any other private members to the class, and the only public members you should add are the operators to overload. The code should compile without errors or warnings with the options `g++ -Wall -std=c++17 -g`. You should not use pointers, only references.

## Sample input/output:

### 1
[input](t1.input):
```
0
{r: 1.00i: 1.01}
{r: 1e2 i: -0.2}
{r: 0i: 0}
{r: 0i: 0]

```

[output](t1.output):
```
{r: 2i: 2.01}
{r: 101i: 0.8}
{r: 1i: 1}
{r: 101i: 0.81}

```

### 2
[input](t2.input):
```
-1
3
1
complex.txt

```

[`complex.txt`](complex.txt):
```
{r: 1.00i: 1.01}
{r: 1e2 i: -0.2}
{r: 0i: 0}
{r: 0i: 0]

```

[output](t2.output):
```
{r: 2i: 2.01}
{r: 101i: 0.8}
{r: 1i: 1}
{r: 101i: 0.81}

```

### 3
[input](t3.input)
```
1
nonexistent.txt

```

[output](t3.output)
```
```
