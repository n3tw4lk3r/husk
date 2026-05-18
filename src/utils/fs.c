#include "utils/fs.h"

#include <fcntl.h>
#include <unistd.h>

#include <stdio.h>
#include <string.h>

int write_file(const char *path, const char *content) {
    int fd = open(path, O_WRONLY);

    if (fd < 0) {
        perror("open");
        return -1;
    }

    ssize_t len = strlen(content);

    if (write(fd, content, len) != len) {
        perror("write");

        close(fd);
        return -1;
    }

    close(fd);

    return 0;
}

