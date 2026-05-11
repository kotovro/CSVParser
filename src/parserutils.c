#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include "parserutils.h"

bool safe_strtol(const char *str, long *out_val) {
    if (str == NULL || *str == '\0') return false;

    char *endptr;
    errno = 0; // Reset errno before the call
    
    long val = strtol(str, &endptr, 10);

    // 1. Check for overflow/underflow
    if ((val == LONG_MAX || val == LONG_MIN) && errno == ERANGE) {
        return false;
    }

    // 2. Check if no digits were found
    if (endptr == str) {
        return false;
    }

    // 3. Check for trailing junk (ensure end of string reached)
    // We skip trailing whitespace as many parsers find it acceptable
    while (*endptr != '\0') {
        if (!((unsigned char)*endptr == ' ' || (unsigned char)*endptr == '\t' || 
              (unsigned char)*endptr == '\n' || (unsigned char)*endptr == '\r')) {
            return false; // Found a non-whitespace character
        }
        endptr++;
    }

    *out_val = val;
    return true;
}

long get_operator_position(const char *formula) {
    for (size_t i = 1, len = strlen(formula); i < len; i++) {
        char c = formula[i];
        if (c == '+' || c == '-' || c == '*' || c == '/') {
            return i;
        }
    }
    return -1; // No operator found
}

bool is_formula_valid(char *formula)
{
    long operator_position = get_operator_position(formula);
    if (operator_position == -1) {
        return false; // No operator found
    }

    for (size_t i = operator_position + 1, len = strlen(formula); i < len; i++) {
        char c = formula[i];
        if (c == '+' || c == '-' || c == '*' || c == '/') {
            if (c != '-' || operator_position != (long)(i - 1)) {
                return false; // More than one operator found or invalid operator sequence
            }
        }
    }
    return operator_position != (long)(strlen(formula) - 1);
}

bool try_get_operands(const char *formula, char **out_operand1, char **out_operand2, char *out_operator) {
    if (!is_formula_valid((char *)formula)) {
        return false;
    }

    int operator_position = get_operator_position(formula);
    
    // Extract operands and operator
    *out_operand1 = malloc(operator_position + 1); 
    strncpy(*out_operand1, formula, operator_position);
    (*out_operand1)[operator_position] = '\0'; // Null-terminate the string
    
    *out_operator = formula[operator_position];

    *out_operand2 = malloc(strlen(formula) - operator_position + 1); 
    strcpy(*out_operand2, formula + operator_position + 1);
    
    return true;
}


