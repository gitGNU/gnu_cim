#
# Include both source and build directories.
#
include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

#
# Check for system include files.
#
include(CheckIncludeFile)
check_include_file("string.h" HAVE_STRING_H)

#
# Need Bison parser.
#
find_package(BISON REQUIRED)
bison_target(parser parser.y "${CMAKE_CURRENT_BINARY_DIR}/parser.c")

#
# Build 'cim' binary.
#
add_executable(cim
    error.c
    pargen.c
    name.c
    dekl.c
    linegen.c
    strgen.c
    extspec.c
    transcall.c
    expgen.c
    getopt1.c
    getopt.c
    parser.c
    lex.c
    filelist.c
    newstr.c
    cimcomp.c
    mellbuilder.c
    expbuilder.c
    sentbuilder.c
    sentchecker.c
    expchecker.c
    computeconst.c
    sentgen.c
    obstack.c
    mapline.c
    senttrans.c
    salloc.c
    passes.c
    dump.c
    "${CMAKE_CURRENT_BINARY_DIR}/parser.c"
)
target_link_libraries(cim PUBLIC m)
#target_compile_options(cim PRIVATE
#    -Wall -g -O3 -ffast-math -fomit-frame-pointer -DHAVE_STRING_H=${HAVE_STRING_H}
#)
install(TARGETS cim DESTINATION bin)
