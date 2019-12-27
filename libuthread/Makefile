# Target library
.DEFAULT_GOAL := libuthread.a
lib := libuthread.a
files := queue.o uthread.o context.o preempt.o
all: $(lib)

## TODO: Phase 1.1

$(files): $(files:.o=.c) $(files:.o=.h)
	gcc -c  $(files:.o=.c) $(files:.o=.h)

$(lib): queue.o uthread.o context.o preempt.o
	ar rcs $(lib) $(files)
 
clean:
	rm -f $(lib) $(files)
	rm -f *.h.gch
