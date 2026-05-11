#ifndef __TABLE__
#define __TABLE__

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Cell Cell;
typedef struct HeaderCell HeaderCell;
typedef struct Line Line;
typedef struct Table Table;

typedef enum {
    CELL_VALUE_UNPARSED = 0,
    CELL_VALUE_PENDING = 1,
    CELL_VALUE_PARSED = 2
} CellValueState;

struct Cell {
    char *data;
    CellValueState value_state;
    long value;
};

struct HeaderCell {
    HeaderCell *next;
    char *column_name;
};

struct Line {
    Cell *cells;
    long line_number;
    Line *next;
};

struct Table {
    HeaderCell *first_header_cell;
    Line *first_line;
    size_t line_count;
    size_t column_count;
};

Table* create_table(FILE *file, char delimiter, char **error_message);
void parse_table(Table *table, char **error_message);
int create_header(const char *header_line, HeaderCell *header_cell, char delimiter, char **error_message);
void add_row(Table *table, const char *line_str, char delimiter, char **error_message);
Cell* create_cell(const char *name, const char *data);

void free_table(Table *table);

void print_table(Table *table);

#endif // __TABLE__