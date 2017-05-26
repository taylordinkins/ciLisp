A C translator for a Lisp-like functional programming language dubbed "ciLisp".

This project was for my Programming Languages undergraduate course at CSU Channel Islands.

Utilized are flex and Bison for scanning/parsing tokens. Allowed are variable declarations using "let" keyword, and function processing for "neg", "abs", "exp", "sqrt", "add", "sub", "mult", "div", "remainder", "log", "pow", "max", "min", "exp2", "cbrt", "hypot", "print", "rand", and "read".

An example of a function call would be (mult 2 3)
Yielding: 6

A variable declaration would be (let (int a 6))
Yielding the equivalent assignment: int a = 6;