# Parent project does not export its library target, so this CML implicitly
# depends on being added from it, i.e. the testing is done only from the build
# tree and is not feasible from an install location

project(bookish-carnivalTests LANGUAGES CXX)

# ---- Tests ----

add_executable(bookish-carnival_test source/bookish-carnival_test.cpp)
target_link_libraries(bookish-carnival_test PRIVATE bookish-carnival_lib)
target_compile_features(bookish-carnival_test PRIVATE cxx_std_20)

add_test(NAME bookish-carnival_test COMMAND bookish-carnival_test)

# ---- End-of-file commands ----

add_folders(Test)
