# Parent project does not export its library target, so this CML implicitly
# depends on being added from it, i.e. the testing is done only from the build
# tree and is not feasible from an install location

project(irregulariaBenchmarks LANGUAGES CXX)

find_package(benchmark REQUIRED HINTS "/usr/local/lib/cmake")

# ---- Benchmarks ----

set(BENCH_FILES 
    source/max.cpp 
    source/xpy.cpp
)

foreach(BENCH_FILE ${BENCH_FILES})
    get_filename_component(BENCH_NAME ${BENCH_FILE} NAME_WLE)
    add_executable(irregularia_${BENCH_NAME} ${BENCH_FILE})

    target_link_libraries(irregularia_${BENCH_NAME} PRIVATE 
        irregularialib 
        benchmark::benchmark
    )
    target_include_directories(irregularia_${BENCH_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>)
    target_compile_features(irregularia_${BENCH_NAME} PRIVATE cxx_std_20)
    target_compile_options(irregularia_${BENCH_NAME} PRIVATE ${ACCBENCH_NVCXX_FLAGS})
    target_link_options(irregularia_${BENCH_NAME} PRIVATE ${ACCBENCH_NVCXX_FLAGS})
endforeach()
