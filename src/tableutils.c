#include <stdlib.h>

#include "tableutils.h"


long get_column_index(HeaderCell *first_header_cell, const char *column_name)
{
    long index = 0;
    HeaderCell *current = first_header_cell;
    while (current) {
        if (strcmp(current->column_name, column_name) == 0) {
            return index;
        }
        current = current->next;
        ++index;
    }
    return -1; // Not found
}

Cell* get_cell_by_position(Table *table, long column_index, long line_number)
{
    Line *current_line = table->first_line;
    while (current_line) {
        if (current_line->line_number == line_number) {
            return &current_line->cells[column_index];
        }
        current_line = current_line->next;
    }
    return NULL;
}

char* get_cell_reference_name(long column_index, long line_number, HeaderCell *first_header_cell)
{
    HeaderCell *current = first_header_cell;
    long current_index = 0;
    while (current_index != column_index) {
        ++current_index;
        current = current->next;
    }

    char line_number_str[33]; // Enough to hold 64-bit integer in base 10 
    // _ltoa(line_number, line_number_str, 10);
    sprintf(line_number_str, "%ld", line_number);
    size_t name_len = strlen(current->column_name) + strlen(line_number_str) + 1;
    char *reference_name = malloc(name_len);
    snprintf(reference_name, name_len, "%s%s", current->column_name, line_number_str);
    return reference_name;
}