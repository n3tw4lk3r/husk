#define _GNU_SOURCE

#include "core/container.h"

#include <limits.h>
#include <sched.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "core/user_namespace.h"
#include "core/uts_namespace.h"
#include "sys/sys.h"
#include "utils/log.h"

enum {
    STACK_SIZE = 1024 * 1024
};

static char child_stack[STACK_SIZE];

typedef struct child_config {
    const container_config *config;
    int pipe_fd[2];
} child_config;

static int child_main(void *arg) {
    child_config *child_cfg = arg;

    sys_close(child_cfg->pipe_fd[1]);

    char sync;

    if (sys_read(child_cfg->pipe_fd[0], &sync, 1) != 1) {
        log_error("failed to synchronize with parent");
        return EXIT_FAILURE;
    }

    sys_close(child_cfg->pipe_fd[0]);

    uts_namespace_setup(child_cfg->config);

    log_info("executing %s", child_cfg->config->argv[0]);
    sys_execvp(child_cfg->config->argv[0], child_cfg->config->argv);

    log_errno("execvp(%s)", child_cfg->config->argv[0]);
    return EXIT_FAILURE;
}

int container_run(const container_config *config) {
    child_config child_cfg = {
        .config = config,
    };

    if (sys_pipe(child_cfg.pipe_fd) < 0) {
        log_errno("pipe");
        return EXIT_FAILURE;
    }

    int flags = config->namespaces | SIGCHLD;

    pid_t pid = sys_clone(
        child_main,
        child_stack + STACK_SIZE,
        flags,
        &child_cfg
    );

    if (pid < 0) {
        sys_close(child_cfg.pipe_fd[0]);
        sys_close(child_cfg.pipe_fd[1]);

        log_errno("clone");
        return EXIT_FAILURE;
    }

    log_info("created container process pid=%d", pid);

    sys_close(child_cfg.pipe_fd[0]);

    log_info("configuring user namespace");

    if (user_namespace_setup(pid) < 0) {
        sys_close(child_cfg.pipe_fd[1]);
        sys_waitpid(pid, NULL, 0);
        return EXIT_FAILURE;
    }

    log_info("user namespace configured");

    if (sys_write(child_cfg.pipe_fd[1], "x", 1) != 1) {
        log_errno("write (sync)");
        sys_close(child_cfg.pipe_fd[1]);
        sys_waitpid(pid, NULL, 0);
        return EXIT_FAILURE;
    }

    sys_close(child_cfg.pipe_fd[1]);

    int status;

    if (sys_waitpid(pid, &status, 0) < 0) {
        log_errno("waitpid");
        return EXIT_FAILURE;
    }

    log_info("container exited");

    if (WIFEXITED(status)) {
        return WEXITSTATUS(status);
    }
    return EXIT_FAILURE;
}

