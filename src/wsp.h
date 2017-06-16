#pragma once

typedef struct {
  int size;
  int **distances;
} Cities;

void init_cities(Cities *cities, int size);
void load_cities(char const *path, Cities *cities);
void print_cities(Cities *cities);
//void apply_mask(int threads, Image *image, Image *result, Mask *mask);
void free_cities(Cities *cities);
