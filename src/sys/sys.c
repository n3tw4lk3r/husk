#define _GNU_SOURCE

#include "sys/sys.h"

#include <sched.h>
#include <sys/wait.h>
#include <unistd.h>

pid_t sys_clone(int (*fn)(void *), void *stack, int flags, void *arg) {
    return clone(fn, stack, flags, arg);
}

int sys_pipe(int pipefd[2]) {
    return pipe(pipefd);
}

ssize_t sys_read(int fd, void *buf, size_t count) {
    return read(fd, buf, count);
}

ssize_t sys_write(int fd, const void *buf, size_t count) {
    return write(fd, buf, count);
}

pid_t sys_waitpid(pid_t pid, int *status, int options) {
    return waitpid(pid, status, options);
}

int sys_execvp(const char *file, char *const argv[]) {
    return execvp(file, argv);
}

int sys_sethostname(const char *name, size_t len) {
    return sethostname(name, len);
}

