#pragma once

#include "core/container_config.h"

typedef struct rootfs_ops {
    int (*prepare)(const container_config *);
    int (*activate)(const container_config *);
    int (*cleanup)(const container_config *);
} rootfs_ops;

int rootfs_setup(const container_config *config);

