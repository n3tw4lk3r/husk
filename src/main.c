#include "core/container.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "usage: husk <command>\n");
        return EXIT_FAILURE;
    }

    return container_run(&argv[1]);
}

