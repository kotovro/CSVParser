#ifndef __PARSER__
#define __PARSER__

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <stdbool.h>

bool safe_strtol(const char *str, long *out_val);
bool calculate_cell_values(const char *str, long *out_val);

#endif // __PARSER__