CC = clang
OPTIONS = -O1 -foptimize-sibling-calls -Wall -Wextra -Wno-unused-parameter -g

ifeq ($(CC), gcc)
	LOG = -fdump-tree-optimized
else ifeq ($(CC), g++)
	LOG = -fdump-tree-optimized
endif

default:
	@printf "Options are:\n\tno_threading\n\tcontinuation_passing\n\
	\tswitch_statement\n\tcomputed_goto\n\tbenchmark\n\tclean\n"
no_threading: no_threading.c threaded_code.h
	$(CC) no_threading.c $(OPTIONS) $(LOG) -o no_threading
continuation_passing: continuation_passing.c threaded_code.h
	$(CC) continuation_passing.c $(OPTIONS)	$(LOG) -o continuation_passing
switch_statement: switch_statement.c threaded_code.h
	$(CC) switch_statement.c $(OPTIONS) $(LOG) -o switch_statement
computed_goto: computed_goto.c threaded_code.h
	$(CC) computed_goto.c $(OPTIONS) $(LOG) -o computed_goto
benchmark: no_threading continuation_passing switch_statement computed_goto
	./no_threading ; ./continuation_passing ; ./switch_statement ;\
	./computed_goto
clean:
	rm continuation_passing 2> /dev/null || true
	rm no_threading 2> /dev/null || true
	rm switch_statement 2> /dev/null || true
	rm computed_goto 2> /dev/null || true
	rm *.c.* 2> /dev/null || true
