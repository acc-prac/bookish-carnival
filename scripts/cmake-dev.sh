#!/usr/bin/env bash
CXX=$(which nvc++) cmake --preset=dev -DBUILD_MCSS_DOCS=OFF -Wno-dev
