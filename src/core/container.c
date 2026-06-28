#define _GNU_SOURCE

#include "core/container.h"

#include <limits.h>
#include <sched.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "sys/sys.h"
#include "utils/fs.h"
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

    sys_sethostname(
        child_cfg->config->hostname,
        strlen(child_cfg->config->hostname)
    );

    log_info("executing %s", child_cfg->config->argv[0]);
    sys_execvp(child_cfg->config->argv[0], child_cfg->config->argv);

    log_errno("execvp(%s)", child_cfg->config->argv[0]);
    return EXIT_FAILURE;
}

static int setup_uid_gid_map(pid_t pid) {
    char path[PATH_MAX];
    char map[256];

    uid_t uid = sys_getuid();
    gid_t gid = sys_getgid();

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
        log_errno("clone");
        return EXIT_FAILURE;
    }

    log_info("created container process pid=%d", pid);

    sys_close(child_cfg.pipe_fd[0]);

    log_info("configuring uid/gid mappings");

    if (setup_uid_gid_map(pid) < 0) {
        sys_close(child_cfg.pipe_fd[1]);
        sys_waitpid(pid, NULL, 0);
        return EXIT_FAILURE;
    }

    log_info("uid/gid mappings configured");

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

