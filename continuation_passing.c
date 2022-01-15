#include "threaded_code.h"

//I really with I could avoid the use of void* and casts here, but
//this typedef would be recursive otherwise.
typedef void *program;
typedef void (*func_ptr)(unsigned, unsigned,  program);

void next(unsigned ip, unsigned i, program prog)
{
	ip += 1;
	func_ptr *next_func = (func_ptr*)prog;
	next_func[ip](ip, i, prog);
}

void begin(unsigned ip, unsigned i, program prog)
{
	printf("Starting run:\n");
	next(ip, i, prog);
}

void incr_i(unsigned ip, unsigned i, program prog)
{
	i += 1;
	next(ip, i, prog);
}

void print_i(unsigned ip, unsigned i, program prog)
{
	if (i % PRINT_INTERVAL == 0) {
		printf("* ");
		fflush(stdout);
	}
	next(ip, i, prog);
}

void branch(unsigned ip, unsigned i, program prog)
{
	if (i == MAX_ITERATIONS)
		ip += 1;
	next(ip, i, prog);
}

void loop(unsigned ip, unsigned i, program prog)
{
	ip = 0;
	next(ip, i, prog);
}

void quit(unsigned ip, unsigned i, program prog)
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
		quit
	};

	printf("Threaded code demo: continuation passing.\n");

	double average = 0;
	for(int j = 0; j < SAMPLE_NUM; ++j) {
		time_t start_time = time(NULL);
		prog[0](0, 0, (void*)prog);
		time_t end_time = time(NULL);
		average += (end_time - start_time);
		printf("\nTime elapsed: %lus\n", end_time - start_time);
	}
	average /= SAMPLE_NUM;
	printf("Average of %d results: %f s\n\n", SAMPLE_NUM, average);
	return 0;
}
