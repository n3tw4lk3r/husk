#pragma once

int log_init(const char *path);
void log_close(void);

void log_info(const char *fmt, ...);
void log_warn(const char *fmt, ...);
void log_error(const char *fmt, ...);

void log_errno(const char *fmt, ...);

