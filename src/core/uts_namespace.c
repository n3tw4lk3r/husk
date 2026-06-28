#include "core/uts_namespace.h"

#include <string.h>

#include "sys/sys.h"

int uts_namespace_setup(const container_config *config) {
    return sys_sethostname(config->hostname, strlen(config->hostname));
}
