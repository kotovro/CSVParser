#include <stdio.h>
#include <stdlib.h>

#include "fileutils.h"

void open_file_if_valid(const char *filename) {
    if (filename == NULL) {
        printf("No filename provided.\n");
        return;
    }

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Failed to open file");
        return;
    }

    printf("File opened successfully: %s\n", filename);

    char *line;
    // Читаем, пока функция не вернет NULL (EOF)
    while ((line = readLongString(file)) != NULL) {
        printf("%s\n", line);
        free(line); // Обязательно освобождаем память
    }

    fclose(file);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    open_file_if_valid(argv[1]);

    
    return 0;
}