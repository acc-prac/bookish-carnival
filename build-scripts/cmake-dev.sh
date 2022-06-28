#!/usr/bin/env sh
srun -w mp-skl2s24c singularity exec -e --nv /mnt/container_store/acc-prac.sif \
    cmake --preset=dev -DBUILD_MCSS_DOCS=OFF