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

#endif // __PARSER__