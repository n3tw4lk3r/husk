#define _GNU_SOURCE

// Will use custom ASM implementations later

#include "sys/sys.h"

#include <fcntl.h>
#include <sched.h>
#include <sys/mount.h>
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

int sys_open(const char *path, int flags, mode_t mode) {
    return open(path, flags, mode);
}

int sys_close(int fd) {
    return close(fd);
}

uid_t sys_getuid(void) {
    return getuid();
}

gid_t sys_getgid(void) {
    return getgid();
}

pid_t sys_getpid(void) {
    return getpid();
}

pid_t sys_fork(void) {
    return fork();
}

int sys_sigaction(
    int signum,
    const struct sigaction *act,
    struct sigaction *oldact
) {
    return sigaction(signum, act, oldact);
}

int sys_sigemptyset(sigset_t *set) {
    return sigemptyset(set);
}

int sys_mount(
    const char *source,
    const char *target,
    const char *filesystemtype,
    unsigned long flags,
    const void *data
) {
    return mount(source, target, filesystemtype, flags, data);
}

