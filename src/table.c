#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "table.h"
#include "fileutils.h"

void free_header(HeaderCell *header)
{
    while (header) {
        HeaderCell *temp = header;
        header = header->next;
        free(temp->column_name);
        free(temp);
    }
}

bool is_column_name_valid(const char *str)
{
    if (str == NULL || *str == '\0') {
        return false;
    }

    char last = '\0';

    for (int i = 0; str[i] != '\0'; i++) {
        char c = str[i];

        // forbid arithmetic symbols
        if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%') {
            return false;
        }

        last = c;
    }

    // must not end with digit
    if (isdigit((unsigned char)last)) {
        return false;
    }

    return true;
}

Table* create_table(FILE *file, char **error_message)
{
    char *line = readLongString(file);
    HeaderCell *header = malloc(sizeof(HeaderCell));
    create_header(line, header, ',', error_message);

    if (*error_message) {
        free(line);
        free_header(header);
        return NULL;
    }

    HeaderCell *current = header;
    while (current) {
        printf("Column: %s\n", current->column_name);
        current = current->next;
    }
    
    // // Читаем, пока функция не вернет NULL (EOF)
    // while ((line = readLongString(file)) != NULL) {
    //     parse_line(line); // Предполагается, что эта функция будет реализована для обработки строки
    //     printf("%s\n", line);
    //     free(line); // Обязательно освобождаем память
    // }

    Table *table = malloc(sizeof(Table));
    // if (!table) return NULL;
    // table->header = NULL;
    // table->lines = NULL;
    // table->line_count = 0;
    // table->column_count = 0;

    // char *line;
    // while ((line = readLongString(file)) != NULL) {
    //     if (table->line_count == 0) {
    //         table->column_count = create_header(line, &table->header, ',');
    //         if (table->column_count < 0) {
    //             free(line);
    //             free_table(table);
    //             return NULL; // Handle header creation failure
    //         }
    //     } else {
    //         // Create cells and link with previous lines
    //     }
    //     free(line); // Free the line after processing
    //     table->line_count++;
    // }

    return table;
}

int create_header(const char *header_line, HeaderCell *header, char delimiter, char **error_message)
{
    if (*header_line != delimiter) {
        *error_message = "First column name should be empty.";
        return -1;
    }
    char *tmp_header = malloc(strlen(header_line) + 1);
    if (!tmp_header) {
        *error_message = "Failed to allocate memory for header.";
        return -1;
    }
    strcpy(tmp_header, header_line);
    char *token = strtok(tmp_header, &delimiter);
    HeaderCell *current = header;
    int header_size = 1;

    while (token) {

        if (!is_column_name_valid(token)) {
            char *message = "Invalid column name: ";
            *error_message = malloc(strlen(message) + strlen(token) + 1);
            sprintf(*error_message, "%s%s", message, token); // Append the invalid name to the error message
            free(tmp_header);
            return -1;
        }
       
        current->column_name = malloc(strlen(token) + 1);
        if (!current->column_name) {
            free(tmp_header);
            *error_message = "Failed to allocate memory for column name.";
            return -1; // Handle memory allocation failure
        }
        strcpy(current->column_name, token);
        token = strtok(NULL, &delimiter);
        if (token) {
            current->next = malloc(sizeof(HeaderCell));
            if (!current->next) {
                *error_message = "Failed to allocate memory for header cell.";
                return -1; // Handle memory allocation failure
            }
            current = current->next;
            current->next = NULL;
        }
        ++header_size;
    }

    int deliemeter_count = 0;
    for (int i = 0, len = strlen(header_line); i < len; i++) {
        if (header_line[i] == delimiter) {
            deliemeter_count++;
        }
    }
    if (deliemeter_count + 1 != header_size) {
        free(tmp_header);
        *error_message = "Too many empty column names.";
        return -1; 
    }

    free(tmp_header);
    return header_size; // Success
}   
