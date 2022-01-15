Threaded code is a technique used in the implementation of virtual machines.
It avoids the overhead of calling subroutines repeatedly by 'threading' them
together with jumps. This was originally done in assembly, where it is the
most straightforward to implement. In C, it is harder to implement
threaded code that is also performant. For an explanation of the different
techniques used, see
https://www.complang.tuwien.ac.at/forth/threaded-code.html .

These files are a test of three different techniques for produced fast threaded
code. Each .c file implements a simple VM that counts to 2 billion, printing
a star whenever the loop counter is evenly divisible by 200 million. The
test settings can be changed in threaded_code.h. They all can be compiled and
ran with 'make benchmark'.

Test results:

I tested these VMs on a desktop computer running Debian with an Intel 
Core i3-3220 CPU at 3.30GHz. Each was run 5 times, then averaged.
This was then performed again at different optimization levels.

Clang 7.0.1		-O0	-O1	-O2	-O3	-Ofast
--------------------------------------------------------------
no threading (control)	32.2	18.4	16.4	16.2	45.8
computed goto		18.8	5.4	 5.4	 5.2	 5.4
continuation passing	 *	7.8	 5.6	 5.8	 4.6
switch statement	22.0	13.4	 7.2	 6.6	 6.6	
--------------------------------------------------------------

GCC 8.3.0		-O0	-O1	-O2	-O3	-Ofast
--------------------------------------------------------------
no threading (control)	30.4	17.8	33.2	14.8	74.2
computed goto		15.8	 6.8	 4.8	 4.8	 5.0
continuation passing	 *	 9.0	 6.2	 6.2	 6.2
switch statement	22.0	13.6	12.8	24.6	24.2
--------------------------------------------------------------

ICC 2021.5.0		-O0	-O1	-O2	-O3	-Ofast
--------------------------------------------------------------
no threading (control)	39.0	20.4	19.8	20.4	21.0
computed goto		23.8	 5.2     5.4     5.2	 5.2
continuation passing	 *	 8.8	 6.4	 6.4	 6.6
switch statement	26.4	15.4	18.4	18.4	17.6
--------------------------------------------------------------

*Requires -foptimize-sibling-calls and an optimization level to be set.
Otherwise, a stack overflow will result.
