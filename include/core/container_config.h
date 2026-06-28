#pragma once

typedef struct container_config {
    char **argv;             /* command to execute */

    const char *hostname;    /* container hostname */
    const char *rootfs;      /* root filesystem */

    unsigned int namespaces; /* clone namespace flags */
} container_config;

void container_config_init(container_config *config);

int container_config_set_command(container_config *config, char **argv);
int container_config_set_hostname(container_config *config, const char *hostname);
int container_config_set_rootfs(container_config *config, const char *rootfs);

int container_config_enable_namespace(container_config *config, unsigned int ns);

int container_config_build(container_config *config);

