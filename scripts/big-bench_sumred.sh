#!/usr/bin/env bash
set -o xtrace

repdir="benchmark/reports"
vizdir="benchmark/vizes"

mkdir -p "${repdir}" "${vizdir}"

#"sumred-host"

for benchmark in "sumred-device-1" "sumred-device-2" "sumred-device-3" "sumred-device-4"; do
    build/prod/benchmark/irregularia_sumred \
        --benchmark_filter="${benchmark}" --benchmark_out="${repdir}"/"${benchmark}".csv \
        --benchmark_min_time=0.001 --benchmark_repetitions=100 \
        --benchmark_report_aggregates_only=true --benchmark_out_format=csv
        
done
