#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "wsp.h"

void init_cities(Cities *cities, int size) {
  cities->size = size;
  cities->distances = calloc(size - 1, sizeof(int*));

  for (int x = 0; x < size - 1; x++) {
    cities->distances[x] = calloc(size - 1 - x, sizeof(int));
  }

}

void load_cities(char const *path, Cities *cities) {
  FILE *file;
  file = fopen(path, "r");
  int size;
  if (!fscanf(file, "%d", &size)) exit(1);
  
  init_cities(cities, size);

  for (int y = 0; y < size - 1; y++) {
    for (int x = 0; x < size - y - 1; x++) {
      if (!fscanf(file, "%d", &(cities->distances[x][y]))) {
        fclose(file);
        exit(1);
      }
    }
  }
  fclose(file);
}

void print_cities(Cities *cities) {
  printf("Size: %d\n", cities->size);
  for (int y = 0; y < cities->size - 1; y++) {
    for (int x = 0; x < cities->size - y - 1; x++) {
      printf("%d ", cities->distances[x][y]);
    }
  }
}

void free_cities(Cities *cities) {
  for (int x = 0; x < cities->size - 1; x++) {
    free(cities->distances[x]);
  }
  free(cities->distances);
  free(cities);
}
