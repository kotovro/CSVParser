#include "fileutils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE* open_file_if_valid(const char *filename, char **error_message) {
    if (filename == NULL) {
        char *message = "No filename provided.";
        *error_message = malloc(strlen(message) + 1);
        sprintf(*error_message, "%s", message);
        return NULL;
    }

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        char *message = "Failed to open file: ";
        *error_message = malloc(strlen(message) + strlen(filename) + 1);
        sprintf(*error_message, "%s%s", message, filename);
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
