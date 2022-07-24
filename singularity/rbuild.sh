#!/usr/bin/env bash
singularity exec -e --nv /mnt/container_store/acc-prac.sif \
	bash -c "scripts/cmake-prod.sh && scripts/build-prod.sh"
