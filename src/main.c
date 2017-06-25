#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <math.h>
#include <getopt.h>
#include <time.h>

#include "stack.h"
#include "wsp.h"

static int verbose_flag;
static int no_output_flag;

void usage() {
  puts("Usage: ./wsp [options] matrix.txt");
  puts("Options:");
  puts("  -h, --help: Print this message.");
  puts("  -n, --nproc: Number of MPI processes. Default: 1");
  puts("    , --verbose: Log detailed information.");
  puts("    , --no-output: Don't write the output image.");
}

void vlog(const char *format, ...) {
  if (verbose_flag) {
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
  }
}

int main(int argc, char *argv[]) {

  int c;

  // Default values
  int nproc = 1;

  while (1) {
    static struct option long_options[] =
    {
      {"verbose", no_argument, &verbose_flag, 1},
      {"no-output", no_argument, &no_output_flag, 1},
      {"help",  required_argument, 0, 'h'},
      {"nproc",    required_argument, 0, 'n'},
      {0, 0, 0, 0}
    };
    /* getopt_long stores the option index here. */
    int option_index = 0;

    c = getopt_long (argc, argv, "hn:",
        long_options, &option_index);

    /* Detect the end of the options. */
    if (c == -1)
      break;

    switch (c) {
      case 0:
        /* If this option set a flag, do nothing else now. */
        if (long_options[option_index].flag != 0)
          break;
        vlog("option %s", long_options[option_index].name);
        if (optarg)
          vlog(" with arg %s", optarg);
        vlog("\n");
        break;

      case 'h':
        usage();
        exit(0);

      case 'n':
        nproc = atoi(optarg);
        break;

      case '?':
        /* getopt_long already printed an error message. */
        break;

      default:
        abort ();
    }
  }


  char *matrix_path;
  if (optind < argc) {
    matrix_path = argv[optind];
  } else {
    printf("You have to specify the path to the matrix file.\n");
    usage();
    exit(1);
  }

  // Load the mask that will be applied.
  Cities cities;// = calloc(1, sizeof(Cities));
  vlog("Loading matrix file in %s\n", matrix_path);
  load_cities(matrix_path, cities);

  print_cities(cities);

  Stack stack;

  int path[] = { 0 }; // Starting city
  push(&stack, path, 1, 0);

  //int *path = calloc(cities->size, sizeof(int));

  //for (int i = 0; i < cities->size; i++) path[i] = i;

  //printf("Distance: %d\n", total_distance(cities, path));

  // Let it go, let it goooooo
  //free(path);
  free_cities(cities);
  free_stack(stack);
  return 0;
}
