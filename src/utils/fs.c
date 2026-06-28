#include "utils/fs.h"

#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include "sys/sys.h"
#include "utils/log.h"

int write_file(const char *path, const char *content) {
    int fd = open(path, O_WRONLY);

    if (fd < 0) {
        log_errno("open(%s)", path);
        return -1;
    }

    ssize_t len = strlen(content);

    if (sys_write(fd, content, len) != len) {
        log_errno("write(%s)", path);
        close(fd);
        return -1;
    }

    close(fd);
    return 0;
}

