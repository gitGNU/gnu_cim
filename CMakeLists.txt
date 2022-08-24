cmake_minimum_required(VERSION 2.9)
project(cim)

#
# Create config.h.
#
configure_file(config.h.in config.h @ONLY)

add_definitions(-DHAVE_CONFIG_H)

#
# Build subdirectories.
#
add_subdirectory(src)
#add_subdirectory(lib)
