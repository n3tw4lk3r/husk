#pragma once

#include <sys/types.h>

pid_t sys_clone(int (*fn)(void *), void *stack, int flags, void *arg);

int sys_pipe(int pipefd[2]);

ssize_t sys_read(int fd, void *buf, size_t count);

ssize_t sys_write(int fd, const void *buf, size_t count);

pid_t sys_waitpid(pid_t pid, int *status, int options);

int sys_execvp(const char *file, char *const argv[]);

int sys_sethostname(const char *name, size_t len);

