
#ifndef DANNYPRIME_DEFS_H
#define DANNYPRIME_DEFS_H
#define NUMBER_OF_CHILD_PROCESSES (3)
#define REALLOC_INCREMENT (20)
#define ENOERR 0
#define DECIMAL_BASE 10
#define CHECK_ALLOC_SUCCESS(p) \
    do { \
        if (NULL == p) { \
            perror("NO MALLOC FOR YOU!"); \
            exit(1); \
        } \
    } while(0)

#define WAIT_FOR_CHILDREN() \
    do { \
    for (;;) { \
        errno = ENOERR; \
        if ((cpid = wait(NULL)) == -1) { \
            if (errno == ECHILD) { \
                break; \
            } else { \
                perror("Wait error"); \
            } \
        } \
    } \
    } while (0)

typedef int c_pipes[NUMBER_OF_CHILD_PROCESSES][2];

#endif //DANNYPRIME_DEFS_H
