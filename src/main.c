#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <getopt.h>
#include <time.h>

#include "utils.h"
#include "tsp.h"
#include "cities.h"

void usage() {
  puts("Usage: ./wsp [options] matrix.txt");
  puts("Options:");
  puts("  -h, --help: Print this message.");
  puts("    , --verbose: Log detailed information.");
  puts("    , --no-output: Don't write the output image.");
}



int main(int argc, char *argv[]) {

  int c;

  while (1) {
    static struct option long_options[] =
    {
      {"verbose", no_argument, &verbose_flag, 1},
      {"no-output", no_argument, &no_output_flag, 1},
      {"help",  required_argument, 0, 'h'},
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
  Cities *cities = calloc(1, sizeof(Cities));
  vlog("Loading matrix file in %s\n", matrix_path);
  load_cities(matrix_path, cities);

  tsp(cities);

  free_cities(cities);
  return 0;
}
