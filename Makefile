# Makefile

CC      = gcc
CFLAGS  = -Wall -Wextra -g -I./include
LFLAGS  = -lm

SRCS    = src/rng.c src/gadgets_basic.c src/gadgets_refresh.c \
          src/gadgets_and.c src/gadgets_conv.c

# -------------------------------------------------------
# Compilation de tous les tests et benchmarks
# -------------------------------------------------------

all: test/test_rng test/test_gadgets_basic test/test_gadgets_refresh \
     test/test_gadgets_and test/test_gadgets_conv test/test_tvla \
     bench/bench_and bench/bench_conv

test/test_rng: test/test_rng.c src/rng.c
	$(CC) $(CFLAGS) -o test/test_rng test/test_rng.c src/rng.c

test/test_gadgets_basic: test/test_gadgets_basic.c src/rng.c src/gadgets_basic.c
	$(CC) $(CFLAGS) -o test/test_gadgets_basic test/test_gadgets_basic.c \
	src/rng.c src/gadgets_basic.c

test/test_gadgets_refresh: test/test_gadgets_refresh.c $(SRCS)
	$(CC) $(CFLAGS) -o test/test_gadgets_refresh test/test_gadgets_refresh.c $(SRCS)

test/test_gadgets_and: test/test_gadgets_and.c $(SRCS)
	$(CC) $(CFLAGS) -o test/test_gadgets_and test/test_gadgets_and.c $(SRCS)

test/test_gadgets_conv: test/test_gadgets_conv.c $(SRCS)
	$(CC) $(CFLAGS) -o test/test_gadgets_conv test/test_gadgets_conv.c $(SRCS)

test/test_tvla: test/test_tvla.c $(SRCS)
	$(CC) $(CFLAGS) -o test/test_tvla test/test_tvla.c $(SRCS) $(LFLAGS)

bench/bench_and: bench/bench_and.c $(SRCS)
	$(CC) $(CFLAGS) -o bench/bench_and bench/bench_and.c $(SRCS)

bench/bench_conv: bench/bench_conv.c $(SRCS)
	$(CC) $(CFLAGS) -o bench/bench_conv bench/bench_conv.c $(SRCS)

# -------------------------------------------------------
# Nettoyage
# -------------------------------------------------------

clean:
	rm -f test/test_rng test/test_gadgets_basic test/test_gadgets_refresh \
	      test/test_gadgets_and test/test_gadgets_conv test/test_tvla \
	      bench/bench_and bench/bench_conv

.PHONY: all clean
