#!/usr/bin/env bash
set -o xtrace

repdir="benchmark/reports"
vizdir="benchmark/vizes"

mkdir -p "${repdir}" "${vizdir}"

for benchmark in "xpy-device" "xpy-host"
do
    build/prod/benchmark/irregularia_benchmark \
        --benchmark_filter="${benchmark}" --benchmark_out="${repdir}"/"${benchmark}".csv \
        --benchmark_min_time=0.001 --benchmark_repetitions=100 \
        --benchmark_report_aggregates_only=true --benchmark_out_format=csv

    python3 scripts/plot.py "${repdir}"/"${benchmark}".csv "${vizdir}"/"${benchmark}".png
done