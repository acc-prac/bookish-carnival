#!/usr/bin/env bash
singularity exec -e --nv /mnt/container_store/acc-prac.sif \
    cmake --build build/prod -j$(nproc)