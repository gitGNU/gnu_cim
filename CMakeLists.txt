cmake_minimum_required(VERSION 3.5)
project(cim)

# By default, build for release with debug info.
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING
        "Default build type: RelWithDebInfo" FORCE)
endif()

add_compile_options(-Werror -Wno-unsequenced) # TODO: -Wall

#
# Create config.h.
#
configure_file(config.h.in config.h @ONLY)

add_definitions(-DHAVE_CONFIG_H)

#
# Build subdirectories.
#
add_subdirectory(src)
add_subdirectory(lib)
