#include <stdarg.h>
#include <stdio.h>

int verbose_flag = 0;
int no_output_flag = 0;

void vlog(const char *format, ...) {
  if (verbose_flag) {
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
  }
}

void output(const char *format, ...) {
  if (!no_output_flag) {
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
  }
}
