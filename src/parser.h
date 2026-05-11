#ifndef __PARSER__
#define __PARSER__

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <stdbool.h>

bool safe_strtol(const char *str, long *out_val);
// static int line_count = 0;
// static int cols_count = 0;

// void parse_line(const char *line);
// void parse_header(const char *header_line);

#endif // __PARSER__