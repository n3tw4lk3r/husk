#define _GNU_SOURCE

#include <sched.h>
#include <sys/wait.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STACK_SIZE (1024 * 1024)

static char child_stack[STACK_SIZE];

typedef struct child_config {
    char **argv;
} child_config;

static int child_main(void *arg) {
    child_config *config = arg;

    sethostname("husk", 4);

    execvp(config->argv[0], config->argv);

    perror("execvp");
    return EXIT_FAILURE;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "usage: husk <command>\n");
        return EXIT_FAILURE;
    }

    child_config config = {
        .argv = &argv[1],
    };

    int flags =
        CLONE_NEWUTS |
        CLONE_NEWPID |
        SIGCHLD;

    pid_t pid = clone(
        child_main,
        child_stack + STACK_SIZE,
        flags,
        &config
    );

    if (pid < 0) {
        perror("clone");
        return EXIT_FAILURE;
    }

    waitpid(pid, NULL, 0);

    return EXIT_SUCCESS;
}

