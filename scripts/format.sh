#!/usr/bin/env bash
find ./source ./include ./test ./benchmark -iname "*.hpp" -o -iname "*.cpp" | xargs clang-format -i
