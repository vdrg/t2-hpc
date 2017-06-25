#include <stdbool.h>
#include <mpi.h>

#include "stack.h"

void master(int num_cities, int nproc);
int find_worker(int *is_working);
void send_work(int worker, Node *node);
bool receive(Stack *stack, int *best, int *done_leafs, bool *is_working, int *waiting);
void receive_result(MPI_Status *status, Stack *stack, int *done_leafs, int *best, int num_cities);
