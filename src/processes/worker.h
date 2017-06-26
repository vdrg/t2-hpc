#pragma once

#include <mpi.h>
#include "../cities.h"

void worker(Cities *cities);
void worker_receive(Cities *cities, MPI_Status *status);
void send_result(int *path, int size);
void send_ready();
