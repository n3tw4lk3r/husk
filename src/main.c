#define _GNU_SOURCE

#include <sched.h>
#include <sys/wait.h>
#include <unistd.h>

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils/fs.h"

#define STACK_SIZE (1024 * 1024)

static char child_stack[STACK_SIZE];

typedef struct child_config {
    char **argv;
    int pipe_fd[2];
} child_config;

static int child_main(void *arg) {
    child_config *config = arg;

    close(config->pipe_fd[1]);

    char sync;

    if (read(config->pipe_fd[0], &sync, 1) != 1) {
        fprintf(stderr, "failed to synchronize with parent\n");
        return EXIT_FAILURE;
    }

    close(config->pipe_fd[0]);

    sethostname("husk", 4);

    execvp(config->argv[0], config->argv);

    perror("execvp");
    return EXIT_FAILURE;
}

static int setup_uid_gid_map(pid_t pid) {
    char path[PATH_MAX];
    char map[256];

    uid_t uid = getuid();
    gid_t gid = getgid();

    snprintf(
        path,
        sizeof(path),
        "/proc/%d/setgroups",
        pid
    );

    if (write_file(path, "deny") < 0) {
        return -1;
    }

    snprintf(
        path,
        sizeof(path),
        "/proc/%d/uid_map",
        pid
    );

    snprintf(
        map,
        sizeof(map),
        "0 %d 1\n",
        uid
    );

    if (write_file(path, map) < 0) {
        return -1;
    }

    snprintf(
        path,
        sizeof(path),
        "/proc/%d/gid_map",
        pid
    );

    snprintf(
        map,
        sizeof(map),
        "0 %d 1\n",
        gid
    );

    if (write_file(path, map) < 0) {
        return -1;
    }

    return 0;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "usage: husk <command>\n");
        return EXIT_FAILURE;
    }

    child_config config = {
        .argv = &argv[1],
    };

    if (pipe(config.pipe_fd) < 0) {
        perror("pipe");
        return EXIT_FAILURE;
    }

    int flags =
        CLONE_NEWUSER |
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

    close(config.pipe_fd[0]);

    if (setup_uid_gid_map(pid) < 0) {
        close(config.pipe_fd[1]);
        waitpid(pid, NULL, 0);

        return EXIT_FAILURE;
    }

    if (write(config.pipe_fd[1], "x", 1) != 1) {
        perror("write");

        close(config.pipe_fd[1]);
        waitpid(pid, NULL, 0);

        return EXIT_FAILURE;
    }

    close(config.pipe_fd[1]);

    waitpid(pid, NULL, 0);

    return EXIT_SUCCESS;
}

