#!/usr/bin/env bash
CXX=$(which nvc++) cmake -S . -B build/prod -DCMAKE_BUILD_TYPE=Release
