# Parent project does not export its library target, so this CML implicitly
# depends on being added from it, i.e. the testing is done only from the build
# tree and is not feasible from an install location

project(irregulariaTests LANGUAGES CXX)


include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


# ---- Tests ----

add_executable(irregularia_test source/irregularia_test.cpp)
add_subdirectory(source)
target_link_libraries(irregularia_test PRIVATE irregularialib gtest_main)
target_compile_features(irregularia_test PRIVATE cxx_std_20)

target_include_directories(
  irregularia_test ${warning_guard} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
)
target_compile_options(irregularia_test PRIVATE ${ACCBENCH_NVCXX_FLAGS})
target_link_options(irregularia_test PRIVATE ${ACCBENCH_NVCXX_FLAGS})

include(GoogleTest)
gtest_discover_tests(irregularia_test)

# add_test(NAME irregularia_test COMMAND irregularia_test)

# ---- End-of-file commands ----

add_folders(Test)
