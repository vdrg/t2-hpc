#pragma once

extern int verbose_flag;
extern int no_output_flag;

void vlog(const char *format, ...);
void output(const char *format, ...);
