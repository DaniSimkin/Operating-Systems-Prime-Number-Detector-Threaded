

#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include "long_io.h"


static int check_prime(long n) {
    long i;

    if (n <= 1) return false;

    if (n <= 3) return true;

    if (n % 2 == 0 || n % 3 == 0) return false;

    for (i = 5; i < sqrtl((double) n); i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) {
            return false;
        }
    }

    return true;
}

int main(int argc, char **argv) {
    int i;
    long *numbers;

    size_t num_of_longs = read_binary_numbers_from_fd(0, &numbers);

    for (i = 0; i < num_of_longs; i++) {
        if (check_prime(numbers[i])) {
            write(1, numbers + i, sizeof(long));
        }
    }

    free(numbers);

    return 0;
}
