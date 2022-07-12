# ---- Declare library ----
add_library(irregularia_cpulib)
target_compile_features(irregularia_cpulib PUBLIC cxx_std_20)
target_compile_options(irregularia_cpulib PRIVATE ${ACCBENCH_NVCXX_FLAGS})
target_link_options(irregularia_cpulib PRIVATE ${ACCBENCH_NVCXX_FLAGS})

add_subdirectory(cpu)

target_include_directories(
  irregularia_cpulib ${warning_guard} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
)


# ---- Declare library ----
add_library(irregularia_devicelib)
target_compile_features(irregularia_devicelib PUBLIC cxx_std_20)
target_compile_options(irregularia_devicelib PRIVATE ${ACCBENCH_NVCXX_FLAGS})
target_link_options(irregularia_devicelib PRIVATE ${ACCBENCH_NVCXX_FLAGS})
add_subdirectory(device)

target_include_directories(
  irregularia_devicelib ${warning_guard} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
)