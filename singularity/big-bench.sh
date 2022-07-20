#!/usr/bin/env bash
set -o xtrace

srun -w mp-skl2s24c singularity exec -e --nv /mnt/container_store/acc-prac.sif \
    bash -c "scripts/big-bench.sh"

repdir="benchmark/reports"
vizdir="benchmark/vizes"

for benchmark in "${repdir}"/*.csv; do
    bname = $(basename "${benchmark}" .csv)
    python3 scripts/plot.py "${benchmark}" "${vizdir}"/"${bname}".png
done
