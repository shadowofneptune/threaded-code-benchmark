#include "threaded_code.h"

typedef enum{
	begin,
	incr_i,
	print_i,
	branch,
	loop,
	quit
} program;

void run_program(unsigned ip, unsigned i, program *prog)
{
	for (;;) {
		switch (prog[ip]) {
		case begin:
			printf("Starting run:.\n");
			break;
		case incr_i:
			i += 1;
			break;
		case print_i:
			if (i % PRINT_INTERVAL == 0) {
				printf("* ");
				fflush(stdout);
			}
			break;
		case branch:
			if (i == MAX_ITERATIONS)
				ip += 1;
			break;
		case loop:
			ip = 0;
			break;
		case quit:
			return;
		}
		ip = ip + 1;
	}
}

int main(void)
{
	program prog[] = {
		begin,
		incr_i,
		print_i,
		branch,
		loop,
		quit
	};

	printf("Threaded code demo: switch statement.\n");

	double average = 0;
	for(int j = 0; j < SAMPLE_NUM; ++j) {
		time_t start_time = time(NULL);
		run_program(0, 0, prog);
		time_t end_time = time(NULL);
		average += (end_time - start_time);
		printf("\nTime elapsed: %lus\n", end_time - start_time);
	}
	average /= SAMPLE_NUM;
	printf("Average of %d results: %f s\n\n", SAMPLE_NUM, average);
	return 0;
}
