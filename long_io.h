

#ifndef DANNYPRIME_LONG_IO_H
#define DANNYPRIME_LONG_IO_H

size_t read_binary_numbers_from_fd(int fd, long **numbers);

int get_single_long_from_fd(FILE *const stream, long *a);

size_t read_string_numbers_from_fd(FILE *const stream, long **numbers);

#endif //DANNYPRIME_LONG_IO_H
