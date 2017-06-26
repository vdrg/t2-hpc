#pragma once

#include <stdbool.h>
#include <mpi.h>

#include "../stack.h"

void master(int num_cities, int nproc);
int find_worker(bool *is_working);
void send_work(int worker, Node *node);
bool master_receive(Stack *stack, int **best, unsigned long long *done_leafs, int num_cities, bool *is_working, int *waiting);
void master_receive_result(MPI_Status *status, Stack *stack, unsigned long long *done_leafs, int **best, int num_cities);
