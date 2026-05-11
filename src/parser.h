#ifndef __PARSER__
#define __PARSER__

#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "parserutils.h"
#include "tableutils.h"

void parse_table(Table *table, char **error_message);
bool calculate_cell_values(const char *str, long *out_val);
bool get_value_from_string(/*Table *table, */ const char *str, char *cell_name, long *out_val, char **error_message);
bool get_value_from_cell(/*Table *table,*/ Cell *cell, char *cell_name, long *out_val, char **error_message);

#endif // __PARSER__