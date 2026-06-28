#define _GNU_SOURCE

#include <sched.h>
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

    container_config config;

    container_config_init(&config);

    container_config_set_command(&config, &argv[1]);

    container_config_enable_namespace(&config, CLONE_NEWUSER);
    container_config_enable_namespace(&config, CLONE_NEWUTS);
    container_config_enable_namespace(&config, CLONE_NEWPID);

    if (container_config_build(&config) < 0) {
        fprintf(stderr, "Invalid container configuration\n");
        log_close();
        return EXIT_FAILURE;
    }

    int ret = container_run(&config);

    log_close();

    return ret;
}

