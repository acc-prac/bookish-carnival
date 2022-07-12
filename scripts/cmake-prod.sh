#!/usr/bin/env bash
singularity exec -e --nv /mnt/container_store/acc-prac.sif \
    cmake -S . -B build/prod -DCMAKE_BUILD_TYPE=Release -DBENCHMARK_DOWNLOAD_DEPENDENCIES=On