#!/usr/bin/env bash
outdir="bench-reports"
mkdir -p "${outdir}"

for benchmark in "xpy"
do
    build/prod/benchmark/irregularia_benchmark \
        --benchmark_filter="${benchmark}" --benchmark_out="${outdir}"/"${benchmark}".csv \
        --benchmark_min_time=0.001 --benchmark_repetitions=100 \
        --benchmark_report_aggregates_only=true --benchmark_out_format=csv
done