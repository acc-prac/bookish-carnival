# Parent project does not export its library target, so this CML implicitly
# depends on being added from it, i.e. the testing is done only from the build
# tree and is not feasible from an install location

project(irregulariaBenchmarks LANGUAGES CXX CUDA)

find_package(benchmark REQUIRED)

# ---- Benchmarks ----

add_executable(irregularia_benchmark source/irregularia_benchmark.cpp)
target_link_libraries(irregularia_benchmark PRIVATE 
    irregularia_cpulib irregularia_devicelib benchmark::benchmark)
target_include_directories(irregularia_benchmark PUBLIC $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>)
target_compile_features(irregularia_benchmark PRIVATE cxx_std_20)