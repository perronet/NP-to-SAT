# NP-to-SAT

This program can:
* Simulate a Turing Machine
* Reduce any [NP](https://en.wikipedia.org/wiki/NP_(complexity)) problem to the [SAT](https://en.wikipedia.org/wiki/Boolean_satisfiability_problem) problem

[Cook-Levin Theorem](https://en.wikipedia.org/wiki/Cook%E2%80%93Levin_theorem)

> Every problem in NP is reducible to a boolean formula that is satisfiable only if the original machine that solves the problem 
> accepts its input.

In order to build the formula you need a maximum amount of computational steps. You can let the program simulate the Turing Machine to find this upper bound for the given input, or just input it yourself.

To test it write your program in `input_program`, according to the syntax.
Then write your input in `input_string` and finally compile and execute
```
gcc parse_dfa.c
./a.out
```
