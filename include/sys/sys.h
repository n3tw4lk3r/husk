#pragma once

#include <signal.h>
#include <sys/types.h>

pid_t sys_clone(int (*fn)(void *), void *stack, int flags, void *arg);

int sys_pipe(int pipefd[2]);

ssize_t sys_read(int fd, void *buf, size_t count);

ssize_t sys_write(int fd, const void *buf, size_t count);

pid_t sys_waitpid(pid_t pid, int *status, int options);

int sys_execvp(const char *file, char *const argv[]);

int sys_sethostname(const char *name, size_t len);

int sys_open(const char *path, int flags, mode_t mode);

int sys_close(int fd);

uid_t sys_getuid(void);

gid_t sys_getgid(void);

pid_t sys_getpid(void);

pid_t sys_fork(void);

int sys_sigaction(
    int signum,
    const struct sigaction *act,
    struct sigaction *oldact
);

int sys_sigemptyset(sigset_t *set);

int sys_mount(
    const char *source,
    const char *target,
    const char *filesystemtype,
    unsigned long flags,
    const void *data
);

int sys_chroot(const char *path);

int sys_chdir(const char *path);

