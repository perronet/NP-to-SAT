# NP-to-SAT

This program can:
* Simulate a Turing Machine
* Reduce any problem in [NP](https://en.wikipedia.org/wiki/NP_(complexity)) to an instance of the [SAT](https://en.wikipedia.org/wiki/Boolean_satisfiability_problem) problem.

The second part is an application of the [Cook-Levin Theorem](https://en.wikipedia.org/wiki/Cook%E2%80%93Levin_theorem), in a nutshell:

> Every problem in NP is reducible in polynomial time to a boolean formula that is satisfiable if and only if the original machine that solves the problem accepts its input.
This also means, by definition, that SAT is [NP-complete](https://en.wikipedia.org/wiki/NP-completeness).

In order to build this formula you need a maximum amount of computational steps, the machine must halt before this bound is reached. You can let the program simulate the Turing Machine to find this upper bound for the given input, or just input it yourself.

It's important to note that if the machine rejects then the resulting upper bound won't be enough for the machine to even try to get a positive answer, so the resulting formula will be trivially unsatisfiable. 

For example: Suppose the machine rejected at the first step and the input was 5 characters long, with a limit of only one step the machine wouldn't even be able to scan the whole input. So if the machine rejects you should try to give an higher upper bound, though because the machine rejected the formula will still be unsatisfiable for any bound.

Be wary that because of the nature of the formula, big upper bounds will make the formula grow. You can't predict how many steps the machine needs to halt. //TODO finish this section

## Quick start
* Write your Turing Machine program in `input_program`, according to the syntax. (TODO add examples dir). 
* Write your input in `input_string`
* Compile with `make`
* Execute with `./run.out`. *To input the maximum computational steps just give it as an argument like* `./run.out 40`
* You can find the output in `formula`. You can use any SAT-solver to check the satisfiability of this formula, [Minisat](https://github.com/niklasso/minisat) is a good one.

## Additional options
`-s sleep_time` Slow mode, used to easily see single steps of the Turing Machine. `sleep_time` is in milliseconds, if it's not given the default will be 500.

`-t` Simulate Turing machine only, don't compute the formula.

Example: `./run.out -s 1000 -t`

You can also compile and run with options like `make run ARGS="-s 1000 -t"`
