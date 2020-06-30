

#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "long_io.h"
#include "defs.h"


static int run_children(c_pipes children_pipes, int parent_pipe[2], long *numbers, size_t n_numbers) {
    pid_t cpid;
    size_t offset = 0;
    size_t longs_to_write;

    for (int child_i = 0; child_i < NUMBER_OF_CHILD_PROCESSES; child_i++) {
        cpid = fork();
        switch (cpid) {
            case -1: // Failed
                perror("Fork failed!");
                exit(1);

            case 0: // I'm a child

                dup2(children_pipes[child_i][0], 0);

                dup2(parent_pipe[1], 1);

                close(children_pipes[child_i][0]);
                close(children_pipes[child_i][1]);
                close(parent_pipe[0]);
                close(parent_pipe[1]);

                char *cmd[] = {"check_prime", (char *) 0};
                execve("./check_prime", cmd, NULL);

                perror("execve failed");
                break;

            default: // I'm the parent
                longs_to_write = n_numbers / NUMBER_OF_CHILD_PROCESSES;

                if (child_i == NUMBER_OF_CHILD_PROCESSES - 1) {
                    longs_to_write += n_numbers % NUMBER_OF_CHILD_PROCESSES;
                }
                write(children_pipes[child_i][1], numbers + offset, longs_to_write * sizeof(long));
                offset += longs_to_write;

                close(children_pipes[child_i][0]);
                close(children_pipes[child_i][1]);

                break;
        }
    }
    close(parent_pipe[1]);


    return 0;
}

static int set_pipes(int parent_pipe[2], c_pipes children_pipes) {
    int i;

    errno = ENOERR;
    if (pipe(parent_pipe) == -1) {
        perror("Failed to create parent pipes");
        exit(1);
    }

    for (i = 0; i < NUMBER_OF_CHILD_PROCESSES; i++) {
        if (pipe(children_pipes[i]) == -1) {
            perror("Failed to create child pipes");
            exit(1);
        }
    }

    return 0;

}

FILE *get_fd(int argc, char **argv) {
    FILE *fd;
    if (argc == 2) {
        fd = fopen(argv[1], "r");
        if (NULL == fd) {
            perror("Failed to open file");
            exit(1);
        }

        printf("Reading numbers from: '%s'\n", argv[1]);
    } else {
        printf("Reading numbers from: stdin\n");
        fd = stdin;
    }

    return fd;
}

int main(int argc, char **argv) {
    int i;
    FILE *fd;
    long *numbers;
    size_t n_numbers;
    pid_t cpid;
    c_pipes children_pipes;
    int parent_pipe[2];

    fd = get_fd(argc, argv);
    n_numbers = read_string_numbers_from_fd(fd, &numbers);

    set_pipes(parent_pipe, children_pipes);

    run_children(children_pipes, parent_pipe, numbers, n_numbers);
    free(numbers);
    numbers = NULL;

    WAIT_FOR_CHILDREN();

    n_numbers = read_binary_numbers_from_fd(parent_pipe[0], &numbers);
    close(parent_pipe[0]);
    for (i = 0; i < n_numbers; i++) {
        printf("%li ", numbers[i]);
    }
    printf("\n");
    free(numbers);

    if (fd != stdin) {
        fclose(fd);
    }

    return 0;

}