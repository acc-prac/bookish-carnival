#!/usr/bin/env bash
set -o xtrace

srun -w mp-skl2s24c singularity exec -e --nv /mnt/container_store/acc-prac.sif \
    bash -c "scripts/big-bench.sh"

scripts/plot-benchmarks.sh