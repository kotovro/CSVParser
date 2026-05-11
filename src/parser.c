#include "parser.h"

bool safe_strtol(const char *str, long *out_val) 
{
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

// bool calculate_cell_values(const char *str, long *out_val)
// {

    
// }