
NPROC=$1

shift

mpiexec -n "$NPROC" tsp $@
