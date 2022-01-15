#include "threaded_code.h"
#include <stdio.h>
#include <time.h>

//I really with I could avoid the use of void* and casts here, but
//this typedef would be recursive otherwise.

typedef enum{
	begin, incr_i, print_i, branch, loop, quit
} program;

void run_program(unsigned ip, unsigned i, program *prog)
{
	void* dispatcher[] = {
		&&begin,
		&&incr_i,
		&&print_i,
		&&branch,
		&&loop,
		&&quit
	};

	goto *dispatcher[prog[ip]];

begin:
	printf("Starting run:\n");
	goto *dispatcher[prog[++ip]];
incr_i:
	i += 1;
	goto *dispatcher[prog[++ip]];
print_i:
	if (i % PRINT_INTERVAL == 0) {
		printf("* ");
		fflush(stdout);
	}
	goto *dispatcher[prog[++ip]];
branch:
	if (i == MAX_ITERATIONS)
		ip += 1;
	goto *dispatcher[prog[++ip]];
loop:
	ip = 0;
	goto *dispatcher[prog[++ip]];
quit:
	return;
}

int main(void)
{
	program prog[] = {
		begin,
		incr_i,
		print_i,
		branch,
		loop,
		quit,
	};

	printf("Threaded code demo: computed goto.\n");

	double average = 0;
	for(int j = 0; j < SAMPLE_NUM; ++j) {
		time_t start_time = time(NULL);
		run_program(0, 0, prog);
		time_t end_time = time(NULL);
		average += (end_time - start_time);
		printf("\nTime elapsed: %lus\n", end_time - start_time);
	}
	average /= SAMPLE_NUM;
	printf("Average of %d results: %fs\n\n", SAMPLE_NUM, average);
	return 0;
}
