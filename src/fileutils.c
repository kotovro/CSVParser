
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "errors_utils.h"
#include "fileutils.h"

FILE* open_file_if_valid(const char *filename, char **error_message) {
    if (filename == NULL) {
        set_error_message(error_message, "%s", "No filename provided.");
        return NULL;
    }

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        set_error_message(error_message, "Failed to open file: %s", filename);
        return NULL;
    }

    return file;
}

char* readLongString(FILE* fp) 
{
    char *str = NULL;
    int current_char;
    size_t len = 0;
    size_t size = 0;

    while ((current_char = fgetc(fp)) != EOF && current_char != '\n') {
        if (current_char == '\r') {
            continue;
        }
        if (len + 1 >= size) {
            size = size == 0 ? 128 : size * 2;
            char *tmp = realloc(str, size);
            if (!tmp) {
                free(str);
                return NULL;
            }
            str = tmp;
        }
        str[len++] = (char)current_char;
    }

    if (len == 0 && current_char == EOF) return NULL;

    if (str) str[len] = '\0';
    else {
        str = malloc(1);
        if(str) str[0] = '\0';
    }

    return str;
}
