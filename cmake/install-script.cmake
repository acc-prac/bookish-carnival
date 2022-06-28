file(
    RELATIVE_PATH relative_path
    "/${irregularia_INSTALL_CMAKEDIR}"
    "/${CMAKE_INSTALL_BINDIR}/${irregularia_NAME}"
)

get_filename_component(prefix "${CMAKE_INSTALL_PREFIX}" ABSOLUTE)
set(config_dir "${prefix}/${irregularia_INSTALL_CMAKEDIR}")
set(config_file "${config_dir}/irregulariaConfig.cmake")

message(STATUS "Installing: ${config_file}")
file(WRITE "${config_file}" "\
get_filename_component(
    _irregularia_executable
    \"\${CMAKE_CURRENT_LIST_DIR}/${relative_path}\"
    ABSOLUTE
)
set(
    BOOKISH_CARNIVAL_EXECUTABLE \"\${_irregularia_executable}\"
    CACHE FILEPATH \"Path to the irregularia executable\"
)
")
list(APPEND CMAKE_INSTALL_MANIFEST_FILES "${config_file}")
