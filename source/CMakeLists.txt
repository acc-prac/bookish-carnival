# ---- Declare library ----
#add_library(irregularia_cpulib INTERFACE)
#target_compile_features(irregularia_cpulib PUBLIC cxx_std_20)
#add_subdirectory(cpu)

#target_include_directories(
#  irregularia_cpulib ${warning_guard} PUBLIC
#  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
#)


# ---- Declare library ----
#add_library(irregularia_devicelib INTERFACE)
#target_compile_features(irregularia_devicelib PUBLIC cxx_std_17)
#add_subdirectory(device)

#target_include_directories(
#  irregularia_devicelib ${warning_guard} PUBLIC
#  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
#)

add_library(irregularialib INTERFACE)
target_sources(irregularialib PRIVATE 
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include/irregularia/accbench_targets.hpp>
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include/irregularia/mi.hpp>
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include/irregularia/milimits.hpp>
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include/irregularia/mitraits.hpp>
)