#include "core/environment.h"

#include <stdlib.h>

int environment_setup(const container_config *config) {
    (void) config;

    return setenv(
        "PATH",
        "/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin",
        1
    );
}
