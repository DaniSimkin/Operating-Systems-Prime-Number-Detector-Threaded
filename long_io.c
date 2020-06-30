

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include "long_io.h"
#include "defs.h"

size_t read_binary_numbers_from_fd(int fd, long **numbers) {
    long iter_long;
    size_t num_of_longs;
    size_t len_of_numbers_array;

    *numbers = (long *) malloc(REALLOC_INCREMENT * sizeof(long));
    CHECK_ALLOC_SUCCESS(*numbers);
    len_of_numbers_array = REALLOC_INCREMENT;

    num_of_longs = 0;
    FILE *const stream = fdopen(fd, "r");
    while (fread(&iter_long, sizeof(long), 1, stream)) {
        *(*numbers + num_of_longs) = iter_long;
        num_of_longs++;

        if (num_of_longs == len_of_numbers_array - 1) {
            *numbers = (long *) realloc(*numbers, (len_of_numbers_array + REALLOC_INCREMENT) * sizeof(long));
            CHECK_ALLOC_SUCCESS(*numbers);

            len_of_numbers_array += REALLOC_INCREMENT;
        }
    }

    return num_of_longs;
}

int get_single_long_from_fd(FILE *const stream, long *a) {
    char buf[1024];
    char *endptr;
    int tmp_char;
    long tmp_long;
    int should_continue = true;
    int i;

    for (;;) {
        tmp_char = fgetc(stream);
        if (!(tmp_char == ' ' || tmp_char == '\n' || tmp_char == '\t')) {
            break;
        }
    }

    for (i = 0; i < sizeof(buf); i++) {
        if (tmp_char == EOF && i == 0) {
            should_continue = false;
        }
        if (tmp_char == ' ' || tmp_char == '\n' || tmp_char == '\t' || tmp_char == EOF) {
            buf[i] = 0;
            break;
        }
        buf[i] = (unsigned char) tmp_char;
        tmp_char = fgetc(stream);
    }
    buf[i + 1] = 0;


    errno = ENOERR;
    tmp_long = strtol(buf, &endptr, DECIMAL_BASE);
    if (errno == ERANGE) {
        perror("Sorry, this number is too small or too large");
        exit(1);
    }

    *a = tmp_long;
    return should_continue;
}

size_t read_string_numbers_from_fd(FILE *const stream, long **numbers) {
    long a;
    size_t len_of_numbers_array;
    size_t num_of_longs;

    *numbers = (long *) malloc(REALLOC_INCREMENT * sizeof(long));
    CHECK_ALLOC_SUCCESS(*numbers);
    len_of_numbers_array = REALLOC_INCREMENT;

    num_of_longs = 0;

    while (get_single_long_from_fd(stream, &a)) {
        *(*numbers + num_of_longs) = a;
        num_of_longs++;

        if (num_of_longs == len_of_numbers_array - 1) {
            *numbers = (long *) realloc(*numbers, (len_of_numbers_array + REALLOC_INCREMENT) * sizeof(long));
            CHECK_ALLOC_SUCCESS(*numbers);

            len_of_numbers_array += REALLOC_INCREMENT;
        }

    }

    return num_of_longs;
}