#include "core/mount_namespace.h"

#include <sys/mount.h>

#include "sys/sys.h"
#include "utils/log.h"

int mount_namespace_setup(void) {
    if (sys_mount(NULL, "/", NULL, MS_REC | MS_PRIVATE, NULL) < 0) {
        log_errno("mount(MS_PRIVATE)");
        return -1;
    }

    return 0;
}
