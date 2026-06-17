#include <stdio.h>
#include <stdlib.h>

#include "core/container.h"
#include "utils/log.h"

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: husk <command>\n");
        return EXIT_FAILURE;
    }
    
    if (log_init("husk.log") < 0) {
        fprintf(stderr, "Failed to open log file\n");
        return EXIT_FAILURE;
    }

    int ret = container_run(&argv[1]);
    
    log_close();

    return ret;
}

