file(
    RELATIVE_PATH relative_path
    "/${bookish-carnival_INSTALL_CMAKEDIR}"
    "/${CMAKE_INSTALL_BINDIR}/${bookish-carnival_NAME}"
)

get_filename_component(prefix "${CMAKE_INSTALL_PREFIX}" ABSOLUTE)
set(config_dir "${prefix}/${bookish-carnival_INSTALL_CMAKEDIR}")
set(config_file "${config_dir}/bookish-carnivalConfig.cmake")

message(STATUS "Installing: ${config_file}")
file(WRITE "${config_file}" "\
get_filename_component(
    _bookish-carnival_executable
    \"\${CMAKE_CURRENT_LIST_DIR}/${relative_path}\"
    ABSOLUTE
)
set(
    BOOKISH_CARNIVAL_EXECUTABLE \"\${_bookish-carnival_executable}\"
    CACHE FILEPATH \"Path to the bookish-carnival executable\"
)
")
list(APPEND CMAKE_INSTALL_MANIFEST_FILES "${config_file}")
