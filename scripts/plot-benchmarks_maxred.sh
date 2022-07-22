#!/usr/bin/env bash
repdir="benchmark/reports"
vizdir="benchmark/vizes"

# shellcheck source=/dev/null
source .venv/bin/activate
for benchmark in "${repdir}"/"maxred"*.csv; do
    bname=$(basename "${benchmark}" .csv)
    python3 scripts/plot.py "${benchmark}" "${vizdir}"/"${bname}".png
done