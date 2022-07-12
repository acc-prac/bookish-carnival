cmake_minimum_required(VERSION 3.17)

include(cmake/prelude.cmake)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "") # works

project(
    irregularia
    VERSION 1.0.0
    DESCRIPTION "Final project submission. Name was chosen by GitHub"
    LANGUAGES CXX
)
#find_package(CUDA REQUIRED)
#enable_language(CUDA)

#include(FindCUDA/select_compute_arch)
#CUDA_DETECT_INSTALLED_GPUS(INSTALLED_GPU_CCS_1)
#string(STRIP "${INSTALLED_GPU_CCS_1}" INSTALLED_GPU_CCS_2)
#string(REPLACE " " ";" INSTALLED_GPU_CCS_3 "${INSTALLED_GPU_CCS_2}")
#string(REPLACE "." "" CUDA_ARCH_LIST "${INSTALLED_GPU_CCS_3}")
#SET(CMAKE_CUDA_ARCHITECTURES ${CUDA_ARCH_LIST})

#message(STATUS "Compiling for CUDA arch: ${CUDA_ARCH_LIST}")

set(CMAKE_CXX_COMPILER nvc++)
set(ACCBENCH_NVCXX_FLAGS -stdpar=gpu -gpu=cc61,cuda11.7)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)


# list(APPEND CUDA_NVCC_FLAGS -stdpar=gpu -gpu=cc61,cuda11.6)

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

# ---- Declare executable ----

add_executable(irregularia_exe source/main.cpp)
add_executable(irregularia::exe ALIAS irregularia_exe)

target_include_directories(
  irregularia_exe ${warning_guard} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
add_subdirectory(source)


set_target_properties(
    irregularia_exe PROPERTIES
    OUTPUT_NAME irregularia
    EXPORT_NAME exe
)

target_compile_features(irregularia_exe PRIVATE cxx_std_20)

target_link_libraries(irregularia_exe PRIVATE 
  irregularia_cpulib
  irregularia_devicelib)

# https://stackoverflow.com/a/72332793... One day...
#set_property(TARGET irregularia_devicelib PROPERTY CUDA_ARCHITECTURES ${CUDA_ARCH_LIST})
#set_property(TARGET irregularia_exe PROPERTY CUDA_ARCHITECTURES ${CUDA_ARCH_LIST})

# ---- Install rules ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
  include(cmake/install-rules.cmake)
endif()

# ---- Developer mode ----

if(NOT irregularia_DEVELOPER_MODE)
  add_subdirectory(benchmark)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  message(
      AUTHOR_WARNING
      "Developer mode is intended for developers of irregularia"
  )
endif()

include(cmake/dev-mode.cmake)
