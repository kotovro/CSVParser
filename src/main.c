#include <stdio.h>
#include <stdlib.h>

#include "fileutils.h"
#include "table.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    char* error_message = NULL;
    FILE* csv_file = open_file_if_valid(argv[1], &error_message);
    if (error_message) {
        printf("Error: %s\n", error_message);
        return 1;
    }

    create_table(csv_file, &error_message);
    if (error_message) {
        printf("Error: %s\n", error_message);
        return 1;
    }

    fclose(csv_file);
    
    return 0;
}