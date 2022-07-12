#!/usr/bin/env bash
CXX=$(which nvc++) cmake --preset=dev -DBUILD_MCSS_DOCS=OFF -D -DIRREGULARIA_CARRY_BIT_POLICY=1 -Wno-dev