#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <mpi.h>

#include "../utils.h"
#include "../cities.h"
#include "../stack.h"
#include "tags.h"
#include "master.h"

void master(int num_cities, int nproc) {

  Stack *stack = calloc(1, sizeof(Stack));

  bool *is_working = (bool *) malloc((nproc - 1) * sizeof(bool)); // true if the worker is doing stuff
  for (int i = 0; i < 5; i++)
    is_working[i] = true;

  int waiting = 0;//nproc - 1; // Number of workers that are waiting

  int start[2] = { 0 }; // The first path is just the node 0, with 0 cost

  push(stack, start, 2);

  unsigned long long done_leafs = 0;

  unsigned long long total_leafs = 1;

  for (int i = num_cities - 1; i > 1; i--)
    total_leafs *= i;

  vlog("Total leafs: %llu\n", total_leafs);
  //printf("%lu\n", total_leafs);

  int *best = NULL; // NULL path

  double start_time = MPI_Wtime();

  while (done_leafs < total_leafs) {
  //while (true) {

    while (!empty(stack) && waiting > 0) {
      /*Node *node = pop(stack);
      if (best == NULL || node->path[node->size - 1] < best[num_cities]) {

        int worker = find_worker(is_working);

        is_working[worker - 1] = true;
        waiting -= 1;
        send_work(worker, node);
        continue;
      }
      free(node);*/

      int worker = find_worker(is_working);

      is_working[worker - 1] = true;
      waiting -= 1;
      send_work(worker, pop(stack));
    }

    master_receive(stack, &best, &done_leafs, num_cities, is_working, &waiting);
  }

  //printf("%llu\n", total_leafs);
  //printf("%d\n", done_leafs);

  // KILL WORKERS
  for (int i = 1; i < nproc; i++) {
    MPI_Send(NULL, 0, MPI_INT, i, DONE_TAG, MPI_COMM_WORLD);
  }

  output("BEST PATH: ");
  print_path_o(best, num_cities + 1);

  printf("%f\n", MPI_Wtime() - start_time);

  free_stack(stack);
  free(is_working);

}

int find_worker(bool *is_working) {
  int i = 0;

  while (is_working[i]) i++;

  return i + 1;
}

void send_work(int worker, Node *node) {
  vlog("Sending path of size %d to worker %d\n", node->size, worker);
  MPI_Send(node->path, node->size, MPI_INT, worker, WORK_TAG, MPI_COMM_WORLD);
  //free(node->path);
  free(node);
}

bool master_receive(Stack *stack, int **best, unsigned long long *done_leafs, int num_cities, bool *is_working, int *waiting) {
  MPI_Status status;
  int available = 0;
  MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &available, &status);

  if (!available) return false;

  switch (status.MPI_TAG) {

    case READY_TAG:

      MPI_Recv(NULL, 0, MPI_INT, status.MPI_SOURCE, READY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      vlog("Master received READY from worker %d\n", status.MPI_SOURCE);

      /*bool sent = false;
      while (!empty(stack)) {
        Node *node = pop(stack);

        if (*best == NULL || node->path[node->size - 1] < (*best)[num_cities]) {
          //printf("Sending work");
          send_work(status.MPI_SOURCE, node);
          sent = true;
          break;
        } else {
          free(node);
        }
      }

      if (!sent) {
        *waiting += 1;
        is_working[status.MPI_SOURCE - 1] = false;
      }*/

      if (!empty(stack)) {
        send_work(status.MPI_SOURCE, pop(stack));
      } else {
        *waiting += 1;
        is_working[status.MPI_SOURCE - 1] = false;
      }

      break;

    case RESULT_TAG:
      vlog("Master received RESULT from worker %d\n", status.MPI_SOURCE);
      master_receive_result(&status, stack, done_leafs, best, num_cities);
      break;

  }

  return true;
}

void master_receive_result(MPI_Status *status, Stack *stack, unsigned long long *done_leafs, int **best, int num_cities) {
  
  int size;
  MPI_Get_count(status, MPI_INT, &size);

  int *result = (int *) malloc(size * sizeof(int));

  MPI_Recv(result, size, MPI_INT, status->MPI_SOURCE, RESULT_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

  int best_cost = *best == NULL ? INT_MAX : (*best)[num_cities];

  if (size == num_cities + 1) { // The worker got to a leaf
    *done_leafs += 1; 
    vlog("Leafs done: %llu\n", *done_leafs);

    if (result[size - 1] < best_cost) {
      *best = result;
      vlog("New best! %d\n", result[size - 1]);
    } else {
      free(result);
    }
    //printf("%lu\n", *done_leafs);
  } else if (result[size - 1] < best_cost) {
    push(stack, result, size);
  } else { // PRUNNING (branch & bound)
    int leafs_prunned = 1;

    for (int i = num_cities - (size - 1); i > 1; i--)
      leafs_prunned *= i;

    vlog("Prunning %d leafs\n", leafs_prunned);
    *done_leafs += leafs_prunned;
    vlog("Leafs done: %llu\n", *done_leafs);

    //printf("%lu\n", *done_leafs);
    free(result);
  }
}
