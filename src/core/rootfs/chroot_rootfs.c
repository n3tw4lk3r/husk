#include "core/rootfs/chroot_rootfs.h"

#include <string.h>

#include "core/container_config.h"
#include "core/rootfs/rootfs.h"
#include "sys/sys.h"
#include "utils/log.h"

static int prepare(const container_config *config) {
    (void) config;
    return 0;
}

static int activate(const container_config *config) {
    if (config->rootfs == NULL) {
        return 0;
    }
    if (sys_chroot(config->rootfs) < 0) {
        log_errno("chroot");
        return -1;
    }

    if (sys_chdir("/") < 0) {
        log_errno("chdir");
        return -1;
    }
    return 0;
}

static int cleanup(const container_config *config) {
    (void) config;
    return 0;
}

const rootfs_ops chroot_rootfs_ops = {
    .prepare = prepare,
    .activate = activate,
    .cleanup = cleanup,
};

