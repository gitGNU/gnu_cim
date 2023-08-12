#
# Include both source and build directories.
#
include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
)

#
# Need Bison parser.
#
find_package(BISON REQUIRED)
bison_target(parser parser.y "${CMAKE_CURRENT_BINARY_DIR}/parser.c")

#
# Build 'gnucim' binary.
#
add_executable(gnucim
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
target_link_libraries(gnucim PUBLIC m)
install(TARGETS gnucim DESTINATION bin)
