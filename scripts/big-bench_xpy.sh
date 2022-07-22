#!/usr/bin/env bash
set -o xtrace

repdir="benchmark/reports"
vizdir="benchmark/vizes"

mkdir -p "${repdir}" "${vizdir}"

#"xpy-host"

for benchmark in "xpy-device-1" "xpy-device-2" "xpy-device-3" "xpy-device-4"; do
    build/prod/benchmark/irregularia_xpy \
        --benchmark_filter="${benchmark}" --benchmark_out="${repdir}"/"${benchmark}".csv \
        --benchmark_min_time=0.001 --benchmark_repetitions=100 \
        --benchmark_report_aggregates_only=true --benchmark_out_format=csv
        
done
