#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

#include "cities.h"

#include "worker.h"
#include "tags.h"

void worker(Cities *cities) {
  //bool *visited = calloc();

  send_ready();

  while (true) {
    receive(cities);
  }
}

void receive(Cities *cities) {
  // Receive work
  // ... path, cost
  MPI_Status status;
  MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

  int size;
  MPI_Get_count(&status, MPI_INT, &size);

  if (size == cities->size - 3) {
    //TODO: optimize
  }

  int *path = (int *) malloc((size + 1) * sizeof(int));
  MPI_Recv(path, size, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

  // Get cities in path
  bool *visited = calloc(cities->size, sizeof(bool));
  for (int i = 0; i < size - 1; i++) {
    visited[path[i]] = true;
  }

  int cost = path[size - 1];
  int next = 0;
  do {
    while (visited[next]) next++;
    path[size - 1] = next;
    path[size] = cost + get_distance(cities, path[size - 1], next);

    send_result(new_path, size + 1);

    next++;
  } while (next < size - 1);

  send_ready();
  free(path);
}

void send_result(int *path, int size) {
  MPI_Send(path, size, MPI_INT, 0, RESULT_TAG, MPI_COMM_WORLD);
}

void send_ready() {
  MPI_Send(NULL, 0, MPI_INT, 0, READY_TAG, MPI_COMM_WORLD);
}
