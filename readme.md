Threaded code is a technique used in the implementation of virtual machines
(VMs). It avoids the overhead of calling subroutines repeatedly by 'threading'
them together with jumps. This was originally done in assembly, where it is the
most straightforward to implement. In C, it is harder to implement
threaded code that is also performant.

These files are a test of three different techniques for produced fast threaded
code. Each .c file implements a simple VM that counts to 2 billion, printing
a star whenever the loop counter is evenly divisible by 200 million. The
test settings can be changed in threaded_code.h. They all can be compiled and
run with 'make benchmark'.

# The VMs

## No threading:

Doesn't use threaded code. This technique is often used to emulate switch
statements in languages like Python. It is going to be the slowest of any
of the VMs, and so acts a a control.

## Computed goto:

The equivalent in Fortran is assigned goto, but the Python VM calls it
computed goto. GCC, Clang, and ICC allow for the location of a label to
be stored in a variable. Since goto can only be used in local scope, the
code implementing the VM instructions has to fit into one large function.

## Continuation-passing:

This is a pretty unusual technique to find in a C program, so I'll spend
more time on it. It starts by calling a function, but then it never
returns from it. That function ends by calling another function,
which also ends by calling another function, and—

This is a recursive technique. As such, it would only be able to keep
calling new functions a few hundred or thousand times before it causes
a stack overflow. However, GCC, Clang, and ICC have support for tail
call optimization, which replaces those function calls with jumps.
This means that it's more or less equivelant to computed goto at an
assembly level.

## Switch statement:

This is the traditional way to implement a VM in C. It is similar in
structure to computed goto, but is regarded as slower due to several
factors. One is that it requires branching, another is that C performs
bounds checking on switch statements. On the other hand, switch
statements are used so often that compiler writers focus on making them
faster.

For more thorough explanations, see
https://www.complang.tuwien.ac.at/forth/threaded-code.html.

So, how do these examples compare to each other?

# Test results:

I tested these VMs on a desktop computer running Debian with an Intel 
Core i3-3220 CPU (Ivy Bridge) at 3.30GHz. Each was run 5 times, then
averaged. This was then performed again at different optimization
levels.

| Clang 7.0.1            | -O0  | -O1  | -O2  | -O3  | -Ofast |
| :--------------------- | ---: | ---: | ---: | ---: | -----: |
| No threading (control) | 32.2 | 18.4 | 16.4 | 16.2 |  45.8  |
| Computed goto          | 18.8 |  5.4 |  5.4 |  5.2 |   5.4  |
| Continuation passing   |  \*  |  7.8 |  5.6 |  5.8 |   4.6  |
| Switch statement       | 22.0 | 13.4 |  7.2 |  6.6 |   6.6  |

| GCC 8.3.0              | -O0  | -O1  | -O2  | -O3  | -Ofast |
| :--------------------- | ---: | ---: | ---: | ---: | -----: |
| No threading (control) | 30.4	| 17.8 | 33.2	| 14.8 |	74.2  |
| Computed goto          | 15.8 |  6.8 |  4.8 |  4.8 |   5.0  |
| Continuation passing   |  \*  |  9.0 |  6.2	|  6.2 |   6.2  |
| Switch statement       | 22.0 |	13.6 | 12.8 |	24.6 |	24.2  |

| ICC 2021.5.0           | -O0  | -O1  | -O2  | -O3  | -Ofast |
| :--------------------- | ---: | ---: | ---: | ---: | -----: |
| No threading (control) | 39.0 | 20.4 | 19.8 | 20.4 |   21.0 |
| Computed goto          | 23.8 |	 5.2 |  5.4 |  5.2 |	 5.2  |
| Continuation passing   |  \*	|  8.8 |  6.4 |	 6.4 |	 6.6  |
| Switch statement       | 26.4 |	15.4 | 18.4 |	18.4 | 	17.6  |

\*Requires -foptimize-sibling-calls and -Og at the minimum to be set.
Otherwise, a stack overflow will result.

# Discussion

Only one of these techniques is guaranteed to work in all compilers, the
switch statement. However, projects like the Python VM have started using
this option only as a fallback, preferring computed goto instead. It's
easy to see why, it's the fastest in almost every test.

Of the techniques, I'd say the continuation-passing one was my favorite
to write. It's much closer to a normal program in structure than the
computed goto or switch statement example, and you could split the code
implementing the VM into separate files without having to rely on macros
to stitch them back together. It's only slightly less performant than
computed goto, while being more in line with structured programming.

That said, clang did shockingly well at optimizing that switch statement.
I'm also on rather old hardware at this point. Give a few more years for
other compilers to catch up, and alternatives to the switch statement
may not even matter.

