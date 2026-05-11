#ifndef __PARSERUTILS__
#define __PARSERUTILS__

#include <stdbool.h>

bool safe_strtol(const char *str, long *out_val);
bool try_get_operands(const char *formula, char **out_operand1, char **out_operand2, char *out_operator);

#endif // __PARSERUTILS__