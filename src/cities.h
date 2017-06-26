#pragma once

typedef struct {
  int size;
  int **distances;
} Cities;

void init_cities(Cities *cities, int size);
void load_cities(char const *path, Cities *cities);
void print_path_o(int *path, int size);
void print_path_v(int *path, int size);
void print_cities(Cities *cities);
int get_distance(Cities *cities, int c1, int c2);
int total_distance(Cities *cities, int *path, int current_min);
void free_cities(Cities *cities);
