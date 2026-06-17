#include "utils/log.h"

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

static void log_message(
    const char *level,
    const char *fmt,
    va_list args
) {
    fprintf(stderr, "[%s] ", level);
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");
}

void log_info(const char *fmt, ...) {
    va_list args;

    va_start(args, fmt);
    log_message("INFO", fmt, args);
    va_end(args);
}

void log_warn(const char *fmt, ...) {
    va_list args;

    va_start(args, fmt);
    log_message("WARN", fmt, args);
    va_end(args);
}

void log_error(const char *fmt, ...) {
    va_list args;

    va_start(args, fmt);
    log_message("ERROR", fmt, args);
    va_end(args);
}

void log_errno(const char *fmt, ...) {
    int saved_errno = errno;

    va_list args;

    va_start(args, fmt);

    fprintf(stderr, "[ERROR] ");
    vfprintf(stderr, fmt, args);
    fprintf(stderr, ": %s\n", strerror(saved_errno));

    va_end(args);
}

