#!/usr/bin/env bash
set -e

build-scripts/cmake-dev.sh
build-scripts/build-dev.sh
build-scripts/run-tests.sh