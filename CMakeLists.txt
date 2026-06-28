cmake_minimum_required(VERSION 3.16)

project(husk C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(
    -Wall
    -Wextra
    -Wpedantic
)

include_directories(include)

add_executable(husk
    src/main.c

    src/core/container.c
    src/core/container_config.c
    src/core/user_namespace.c
    src/core/uts_namespace.c

    src/sys/sys.c

    src/utils/fs.c
    src/utils/log.c
)

