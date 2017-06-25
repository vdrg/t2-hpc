#include <stdbool.h>
#include <stdio.h>
#include <mpi.h>

#include "tags.h"
#include "stack.h"
#include "master.h"

void master(int num_cities, int nproc) {

  Stack *stack = calloc(1, sizeof(Stack));

  bool *is_working = calloc(nproc - 1, sizeof(bool)); // true if the worker is doing stuff

  int waiting = nproc - 1; // Number of workers that are waiting

  int best = 100000000;

  int *start = { 0, 0 }; // The first path is just the node 0, with 0 cost

  push(&stack, start, 2);

  int done_leafs = 0;

  int total_leafs = 1;

  for (int i = num_cities - 1; i > 1; --i)
    total_leafs *= i;

  while (done_leafs < total_leafs) {
    if (!receive(stack, &best, &done_leafs, is_working, &waiting) && !empty(stack) && waiting > 0) {
      int worker = find_worker(is_working);
      
      send_work(worker, pop(stack));
    }
  }

  printf("Best: %d\n", best);

  free_stack(stack);
}

int find_worker(int *is_working) {
  int i = 0;

  while (is_working[i]) i++;

  return i;
}

void send_work(int worker, Node *node) {
  MPI_Send(node->path, node->size, MPI_INT, worker, 0, MPI_COMM_WORLD);
  free(node);
}

bool receive(Stack *stack, int *best, int *done_leafs, bool *is_working, int *waiting) {
  MPI_Status status;
  int available = 0;
  MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &available, &status);

  if (!available) return false;

  switch (status.MPI_TAG) {

    case READY_TAG:

      if (!empty(stack)) {
        send_work(status.MPI_SOURCE, pop(stack));
      } else {
        *waiting++;
        is_working[status.MPI_SOURCE - 1] = false;
      }
      
      break;

    case RESULT_TAG:
      receive_result(&status, stack, best, done_leafs);
      break;

  }

  return true;
}

void receive_result(MPI_Status *status, Stack *stack, int *done_leafs, int *best, int num_cities) {
  
  int size;
  MPI_Get_count(&status, MPI_INT, &size);

  int *result = (int *) malloc(size * sizeof(int));

  MPI_Recv(result, size, MPI_INT, status->MPI_SOURCE, RECEIVE_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

  if (size == num_cities + 1) { // The worker got to a leaf
    *done_leafs++; 

    if (result[size - 1] < *best) {
      *best = result[size - 1];
      // TODO: store the path
    }
  } else if (result[size - 1] < *best) {
    push(stack, result);
  }

  //free(result);
}
