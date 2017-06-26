
NPROC=$1

shift

mpiexec -n "$NPROC" --hostfile hosts tsp $@
