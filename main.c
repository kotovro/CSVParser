#include <stdio.h>
#include <stdlib.h>

#include "src/fileutils.h"
#include "src/table.h"

int main(int argc, char *argv[]) {
    fprintf(stderr, "entered main\n");
    printf("are we here");
    if (argc < 2) {

        fprintf(stderr, "argsinvalid\n");
    
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    char* error_message = NULL;
     fprintf(stderr, "try open file\n");
    FILE* csv_file = open_file_if_valid(argv[1], &error_message);
    fprintf(stderr, "opened file\n");
    if (error_message) {
        printf("got error");
        printf("Error: %s\n", error_message);
        free(error_message);
        return 1;
    }

    fprintf(stderr, "try create table\n");
    Table *table = create_table(csv_file, ',', &error_message);
    fprintf(stderr, "created table\n");
    if (error_message) {
        printf("Error: %s\n", error_message);
        free(error_message);
        fclose(csv_file);
        return 1;
    }
    fclose(csv_file);

    parse_table(table, &error_message);
    if (error_message) {
        printf("Error: %s\n", error_message);
        free(error_message);
        free_table(table);
        return 1;
    }
    
    print_table(table);
    free_table(table);
    
    return 0;
}