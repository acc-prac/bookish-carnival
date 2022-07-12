#!/usr/bin/env bash
singularity exec -e --nv /mnt/container_store/acc-prac.sif \
    cmake --preset=dev -DBUILD_MCSS_DOCS=OFF -Wno-dev