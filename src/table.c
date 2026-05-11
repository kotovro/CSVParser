#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "table.h"
#include "fileutils.h"
#include "parser.h"

void free_header(HeaderCell *header)
{
    while (header) {
        HeaderCell *temp = header;
        header = header->next;
        free(temp->column_name);
        free(temp);
    }
}

void free_line(Line *line, long column_count)
{
    if (!line) return;

    for (int i = 0; i < column_count; i++) {
        free(line->cells[i].inner_name);
        free(line->cells[i].data);
    }
    free(line->cells);
    free(line);
}

void free_table(Table* table) 
{
    if (!table) return;

    free_header(table->first_header_cell);

    Line *current_line = table->first_line;
    while (current_line) {
        Line *next_line = current_line->next;
        free_line(current_line, table->column_count - 1);
        current_line = next_line;
    }

    free(table);
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
    HeaderCell *header_start_cell = malloc(sizeof(HeaderCell));
    int col_count = create_header(line, header_start_cell, ',', error_message);
    free(line);

    if (*error_message) {
        free_header(header_start_cell);
        return NULL;
    }

    Table *table = malloc(sizeof(Table));
    table->first_header_cell = header_start_cell;
    table->column_count = col_count;
    table->line_count = 0;
    table->first_line = NULL;


    // // Читаем, пока функция не вернет NULL (EOF)
    while ((line = readLongString(file)) != NULL) {
        add_row(table, line, ',', error_message);
        free(line);
        if (*error_message) {
            free_table(table);
            return NULL;
        }
    }
    
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

int create_header(const char *header_line, HeaderCell *header_cell, char delimiter, char **error_message)
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
    HeaderCell *current = header_cell;
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
            HeaderCell *temp = header_cell;
            while (temp) {
                if (strcmp(temp->column_name, token) == 0) {
                    char *message = "Duplicate column name: ";
                    *error_message = malloc(strlen(message) + strlen(token) + 1);
                    sprintf(*error_message, "%s%s", message, token);
                    free(tmp_header);
                    return -1;
                }
                temp = temp->next;
            }
        
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

void add_row(Table *table, const char *line_str, char delimiter, char **error_message)
{
    size_t deliemeter_count = 0;
    for (size_t i = 0, len = strlen(line_str); i < len; i++) {
        if (line_str[i] == delimiter) {
            deliemeter_count++;
        }
    }
    if (deliemeter_count + 1 != table->column_count) {
        char *message = "Incorrect number of cells in line: ";
        *error_message = malloc(strlen(message) + strlen(line_str) + 1);
        sprintf(*error_message, "%s%s", message, line_str); // Append the invalid name to the error message
        return; 
    }
    Line *new_line = malloc(sizeof(Line));
    if (!new_line) {
        *error_message = "Failed to allocate memory for new line.";
        return; // Handle memory allocation failure
    }
    new_line->line_number = 0; // This should be set to the actual line number when processing
    new_line->cells = NULL;
    new_line->next = NULL;

    char *tmp_line = malloc(strlen(line_str) + 1);
    if (!tmp_line) {
        *error_message = "Failed to allocate memory for temporary line.";
        free(new_line);
        return; // Handle memory allocation failure
    }
    strcpy(tmp_line, line_str);

    char *token = strtok(tmp_line, &delimiter);
    if (!safe_strtol(token, &new_line->line_number)) {
        char *message = "Invalid line number: ";
        *error_message = malloc(strlen(message) + strlen(token) + 1);
        sprintf(*error_message, "%s%s", message, token); // Append the invalid name to the error message
        free(tmp_line);
        free(new_line);
        return;
    }
    
    if (table->first_line == NULL) {
        table->first_line = new_line;
    } else {
        Line *current = table->first_line;
        Line *last_line = NULL;
        while (current) {
            if (current->line_number == new_line->line_number) {
                char *message = "Duplicate line number: ";
                *error_message = malloc(strlen(message) + strlen(token) + 1);
                sprintf(*error_message, "%s%ld", message, new_line->line_number); // Append the invalid name to the error message
                free(tmp_line);
                free(new_line);
                return;
            }
            last_line = current;
            current = current->next;
        }
        last_line->next = new_line;
    }

    new_line->cells = malloc((table->column_count - 1) * sizeof(Cell)); // Initialize cells to NULL

    HeaderCell *column = table->first_header_cell; 
    
    for (size_t i = 0; i < table->column_count - 1; i++) {
        token = strtok(NULL, &delimiter);
        if (!token) {
            char *message = "Not enough cells in line: ";
            *error_message = malloc(strlen(message) + strlen(line_str) + 1);
            sprintf(*error_message, "%s%s", message, line_str); // Append the invalid name to the error message
            free(tmp_line);
            return;
        }
        new_line->cells[i].inner_name = malloc(strlen(column->column_name) + 1); // Assuming the name is not needed for now
        strcpy(new_line->cells[i].inner_name, column->column_name);

        new_line->cells[i].data = malloc(strlen(token) + 1);
        strcpy(new_line->cells[i].data, token);
        
        column = column->next; // Move to the next column in the header
    }
    token = strtok(NULL, &delimiter);
    if (token) {
        char *message = "Too many cells in line: ";
        *error_message = malloc(strlen(message) + strlen(line_str) + 1);
        sprintf(*error_message, "%s%s", message, line_str); // Append the invalid name to the error message
        free(tmp_line);
        return;
    }
}

void print_table(Table *table)
{
    if (!table) {
        printf("Table is NULL.\n");
        return;
    }

    // Print header
    HeaderCell *current_header = table->first_header_cell;
    while (current_header) {
        printf(",%s", current_header->column_name);
        current_header = current_header->next;
    }
    printf("\n");

    // Print lines
    Line *current_line = table->first_line;
    while (current_line) {
        printf("%ld", current_line->line_number);
        for (size_t i = 0; i < table->column_count - 1; i++) {
            printf(",%s", current_line->cells[i].data);
        }
        printf("\n");
        current_line = current_line->next;
    }

}
