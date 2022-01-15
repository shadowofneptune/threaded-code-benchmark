#include "threaded_code.h"

typedef void (*func_ptr)(unsigned*, unsigned*);

void begin(unsigned *ip, unsigned *i)
{
	printf("Starting run:\n");
}

void incr_i(unsigned *ip, unsigned *i)
{
	*i += 1;
}

void print_i(unsigned *ip, unsigned *i)
{
	if (*i % PRINT_INTERVAL == 0) {
		printf("* ");
		fflush(stdout);
	}
}

void branch(unsigned *ip, unsigned *i)
{
	if (*i == MAX_ITERATIONS)
		*ip += 1;
}

void loop(unsigned *ip, unsigned *i)
{
	*ip = 0;
}

void quit(unsigned *ip, unsigned *i)
{
	return;
}

int main(void)
{
	func_ptr prog[] = {
		begin,
		incr_i,
		print_i,
		branch,
		loop,
		quit,
	};

	printf("Threaded code demo: no threading, subroutines only.\n");

	double average = 0;
	for(int j = 0; j < SAMPLE_NUM; ++j) {
		unsigned ip = 0;
		unsigned i = 0;
		time_t start_time = time(NULL);
		while (prog[ip] != quit) {
			prog[ip](&ip, &i);
			ip += 1;
		}
		time_t end_time = time(NULL);
		average += (end_time - start_time);
		printf("\nTime elapsed: %lus\n", end_time - start_time);
	}
	average /= SAMPLE_NUM;
	printf("Average of %d results: %f s\n\n", SAMPLE_NUM, average);
	return 0;
}
