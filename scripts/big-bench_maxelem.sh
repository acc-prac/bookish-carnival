#!/usr/bin/env bash
set -o xtrace

repdir="benchmark/reports"
vizdir="benchmark/vizes"

mkdir -p "${repdir}" "${vizdir}"

#"maxelem-host"

for benchmark in "maxelem-device-1" "maxelem-device-2" "maxelem-device-3" "maxelem-device-4"; do
    build/prod/benchmark/irregularia_maxelem \
        --benchmark_filter="${benchmark}" --benchmark_out="${repdir}"/"${benchmark}".csv \
        --benchmark_min_time=0.001 --benchmark_repetitions=100 \
        --benchmark_report_aggregates_only=true --benchmark_out_format=csv
        
done
