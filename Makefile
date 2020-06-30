CC=gcc
CFLAGS=-I . -lm
DEPS = long_io.h defs.h
OBJ := long_io.o
MAIN_OBJ := $(OBJ) danny_prime.o
CHECKPRIME_OBJ = $(OBJ) check_prime.o

.PHONY: all

all: check_prime danny_prime

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

check_prime: $(CHECKPRIME_OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

danny_prime: $(MAIN_OBJ) check_prime
	$(CC) -o $@ $(MAIN_OBJ) $(CFLAGS)
	
.PHONY: clean

clean:
	rm *.o check_prime danny_prime

.PHONY: from_stdin from_file

from_stdin: all
	echo "All prime numbers until 1000:"; \
       	seq 1000 | ./danny_prime;

from_file: all
	echo "All prime numbers until 1000:"; \
	seq 1000 > /tmp/1000_nums; \
       	./danny_prime /tmp/1000_nums;

