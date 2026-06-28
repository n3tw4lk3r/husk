#include "core/container_config.h"

#include <stdlib.h>

void container_config_init(container_config *config) {
    config->argv = NULL;
    config->hostname = "husk";
    config->rootfs = NULL;
    config->namespaces = 0;
}

int container_config_set_command(container_config *config, char **argv) {
    config->argv = argv;
    return 0;
}

int container_config_set_hostname(container_config *config, const char *hostname) {
    config->hostname = hostname;
    return 0;
}

int container_config_set_rootfs(container_config *config, const char *rootfs) {
    config->rootfs = rootfs;
    return 0;
}

int container_config_enable_namespace(container_config *config, unsigned int ns) {
    config->namespaces |= ns;
    return 0;
}

int container_config_build(container_config *config) {
    if (config->argv == NULL) {
        return -1;
    }
    return 0;
}

