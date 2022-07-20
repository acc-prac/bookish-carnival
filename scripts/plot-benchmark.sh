#!/usr/bin/env bash
repdir="benchmark/reports"
vizdir="benchmark/vizes"

for benchmark in "${repdir}"/*.csv; do
    bname = $(basename "${benchmark}" .csv)
    python3 scripts/plot.py "${benchmark}" "${vizdir}"/"${bname}".png
done
