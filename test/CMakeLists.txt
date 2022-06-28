# Parent project does not export its library target, so this CML implicitly
# depends on being added from it, i.e. the testing is done only from the build
# tree and is not feasible from an install location

project(irregulariaTests LANGUAGES CXX CUDA)

# ---- Tests ----

add_executable(irregularia_test source/irregularia_test.cpp)
target_link_libraries(irregularia_test PRIVATE irregularia_cpulib irregularia_devicelib)
target_compile_features(irregularia_test PRIVATE cxx_std_17)

target_include_directories(
  irregularia_test ${warning_guard} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
)

add_test(NAME irregularia_test COMMAND irregularia_test)

# ---- End-of-file commands ----

add_folders(Test)
