#define _GNU_SOURCE

#include "core/container.h"

#include <sched.h>
#include <sys/wait.h>
#include <unistd.h>

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "utils/fs.h"
#include "utils/log.h"

enum {
    STACK_SIZE = 1024 * 1024
};

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
        log_error("failed to synchronize with parent");
        return EXIT_FAILURE;
    }

    close(config->pipe_fd[0]);

    sethostname("husk", 4);

    log_info("executing %s", config->argv[0]);
    execvp(config->argv[0], config->argv);
    
    log_errno("execvp(%s)", config->argv[0]);
    return EXIT_FAILURE;
}

static int setup_uid_gid_map(pid_t pid) {
    char path[PATH_MAX];
    char map[256];

    uid_t uid = getuid();
    gid_t gid = getgid();

    snprintf(path, sizeof(path), "/proc/%d/setgroups", pid);

    if (write_file(path, "deny") < 0) {
        return -1;
    }

    snprintf(path, sizeof(path), "/proc/%d/uid_map", pid);
    snprintf(map, sizeof(map), "0 %d 1\n", uid);

    if (write_file(path, map) < 0) {
        return -1;
    }

    snprintf(path, sizeof(path), "/proc/%d/gid_map", pid);
    snprintf(map, sizeof(map), "0 %d 1\n", gid);

    if (write_file(path, map) < 0) {
        return -1;
    }

    return 0;
}

int container_run(char **argv) {
    child_config config = {
        .argv = argv,
    };

    if (pipe(config.pipe_fd) < 0) {
        log_errno("pipe");
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
        log_errno("clone");
        return EXIT_FAILURE;
    }

    log_info("created container process pid=%d", pid);

    close(config.pipe_fd[0]);

    log_info("configuring uid/gid mappings");

    if (setup_uid_gid_map(pid) < 0) {
        close(config.pipe_fd[1]);
        waitpid(pid, NULL, 0);
        return EXIT_FAILURE;
    }

    log_info("uid/gid mappings configured");

    if (write(config.pipe_fd[1], "x", 1) != 1) {
        log_errno("write (sync)");
        close(config.pipe_fd[1]);
        waitpid(pid, NULL, 0);
        return EXIT_FAILURE;
    }

    close(config.pipe_fd[1]);

    int status;

    if (waitpid(pid, &status, 0) < 0) {
        log_errno("waitpid");
        return EXIT_FAILURE;
    }

    log_info("container exited");
    
    if (WIFEXITED(status)) {
        return WEXITSTATUS(status);
    }
    return EXIT_FAILURE;
}

