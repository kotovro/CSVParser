#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "errors_utils.h"

void set_error_message(char** error_message, const char* template, const char* value) {
    *error_message = malloc(strlen(template) + strlen(value) + 1);
    if (!*error_message) {
        free(error_message);
        return; // Handle memory allocation failure
    }
    sprintf(*error_message, template, value);
}