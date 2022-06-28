#!/usr/bin/env bash
set -e

build-scripts/cmake-prod.sh
build-scripts/build-prod.sh
build-scripts/run-benchmark.sh