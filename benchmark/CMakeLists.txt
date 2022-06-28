# Parent project does not export its library target, so this CML implicitly
# depends on being added from it, i.e. the testing is done only from the build
# tree and is not feasible from an install location

project(irregulariaBenchmarks LANGUAGES CXX CUDA)

include(FetchContent)
FetchContent_Declare(googlebenchmark
    URL https://github.com/google/benchmark/archive/refs/tags/v1.6.1.zip)
FetchContent_MakeAvailable(googlebenchmark)

# ---- Benchmarks ----

add_executable(irregularia_benchmark source/irregularia_benchmark.cpp)
target_link_libraries(irregularia_benchmark PRIVATE 
    irregularia_cpulib irregularia_devicelib benchmark::benchmark)
target_compile_features(irregularia_benchmark PRIVATE cxx_std_17)