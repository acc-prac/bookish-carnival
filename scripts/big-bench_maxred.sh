#!/usr/bin/env bash
set -o xtrace

repdir="benchmark/reports"
vizdir="benchmark/vizes"

mkdir -p "${repdir}" "${vizdir}"

#"maxred-host"

for benchmark in "maxred-device-1" "maxred-device-2" "maxred-device-3" "maxred-device-4"; do
    build/prod/benchmark/irregularia_maxred \
        --benchmark_filter="${benchmark}" --benchmark_out="${repdir}"/"${benchmark}".csv \
        --benchmark_min_time=0.001 --benchmark_repetitions=100 \
        --benchmark_report_aggregates_only=true --benchmark_out_format=csv
        
done
