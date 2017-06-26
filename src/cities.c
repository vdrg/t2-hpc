#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "cities.h"
#include "utils.h"

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

int get_distance(Cities *cities, int c1, int c2) {
  int _c1 = c1 < c2 ? c1 : c2;
  int _c2 = c1 < c2 ? c2 : c1;

  return cities->distances[_c2 - _c1 - 1][_c1];
}

void print_path_v(int *path, int size) {
  for (int i = 0; i < size - 1; i++) {
    vlog("%d ", path[i]);
  }
  vlog("| Cost: %d\n", path[size - 1]);
}

void print_path_o(int *path, int size) {
  for (int i = 0; i < size - 1; i++) {
    output("%d ", path[i]);
  }
  output("| Cost: %d\n", path[size - 1]);
}

void print_cities(Cities *cities) {
  for (int y = 0; y < cities->size - 1; y++) {
    for (int x = 0; x < cities->size - y - 1; x++) {
      printf("%d ", cities->distances[x][y]);
    }
    printf("\n");
  }
}

void free_cities(Cities *cities) {
  for (int x = 0; x < cities->size - 1; x++) {
    free(cities->distances[x]);
  }
  free(cities->distances);
  free(cities);
}
