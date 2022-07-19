#!/usr/bin/env bash
srun -w mp-skl2s24c singularity exec -e --nv /mnt/container_store/acc-prac.sif \
    build/prod/benchmark/irregularia_benchmark \
        --benchmark_repetitions=100 \
        --benchmark_report_aggregates_only=true \
        --benchmark_min_time=0.001 --benchmark_out_format=csv \
        --benchmark_out=out.csv