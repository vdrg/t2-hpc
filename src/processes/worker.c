#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <mpi.h>

#include "../cities.h"
#include "../utils.h"

#include "worker.h"
#include "tags.h"

void worker(Cities *cities) {

  send_ready();

  while (true) {
    MPI_Status status;
    MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    
    if (status.MPI_TAG == DONE_TAG) break;

    worker_receive(cities, &status);
  }
}

// Receive a path and expand it or finish
void worker_receive(Cities *cities, MPI_Status *status) {

  int size;
  MPI_Get_count(status, MPI_INT, &size);


  vlog("Worker received path of size: %d\n", size);

  bool one_left = size == cities->size - 1;
  
  int *path = (int *) malloc((size + (one_left ? 2 : 1)) * sizeof(int));
  MPI_Recv(path, size, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

  // Get cities in path
  bool *visited = calloc(cities->size, sizeof(bool));
  for (int i = 0; i < size - 1; i++) {
    visited[path[i]] = true;
  }

  int cost = path[size - 1];
  int next = 0;
  while (true) {

    // Search for a city that hasn't been visited.
    while (next < cities->size && visited[next]) next++;

    // If there are only 2 possible paths, expand them at once.
    if (one_left) {
      int next2 = next + 1;
      while (next2 < cities->size && visited[next2]) next2++;

      path[size - 1] = next;
      path[size] = next2;
      path[size + 1] = cost + get_distance(cities, path[size - 2], next) + get_distance(cities, next, next2);
      send_result(path, size + 2);

      path[size - 1] = next2;
      path[size] = next;
      path[size + 1] = cost + get_distance(cities, path[size - 2], next2) + get_distance(cities, next2, next);
      send_result(path, size + 2);
      break;
    }

    if (next == cities->size) break;

    path[size - 1] = next;
    path[size] = cost + get_distance(cities, path[size - 2], next);

    send_result(path, size + 1);

    next++;
  }

  send_ready();
  free(path);
  free(visited);
}

void send_result(int *path, int size) {
  vlog("Worker sending RESULT: ");
  print_path_v(path, size);
  MPI_Send(path, size, MPI_INT, 0, RESULT_TAG, MPI_COMM_WORLD);
}

void send_ready() {
  vlog("Worker sending READY\n");
  MPI_Send(NULL, 0, MPI_INT, 0, READY_TAG, MPI_COMM_WORLD);
}
