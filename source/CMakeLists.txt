# ---- Declare library ----
add_library(bookish-carnival_cpulib)
target_compile_features(bookish-carnival_cpulib PUBLIC cxx_std_20)
add_subdirectory(cpu)

target_include_directories(
  bookish-carnival_cpulib ${warning_guard} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)


# ---- Declare library ----
add_library(bookish-carnival_devicelib)
target_compile_features(bookish-carnival_devicelib PUBLIC cxx_std_20)
add_subdirectory(device)

target_include_directories(
  bookish-carnival_devicelib ${warning_guard} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)