#include <mpi.h>

#include "stack.h"
#include "cities.h"

void wsp(Cities *cities) {
  int rank, nproc;

  MPI_Init(NULL, NULL);

  MPI_Comm_size(MPI_COMM_WORLD, &nproc);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);


  if (rank == 0) {
    master(cities->size, nproc);
  } else {
    worker(cities);
  }
}





