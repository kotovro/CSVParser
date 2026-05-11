#ifndef __TABLE__
#define __TABLE__

#include <stdbool.h>

typedef struct Cell Cell;
typedef struct HeaderCell HeaderCell;
typedef struct Line Line;
typedef struct Table Table;

struct Cell {
    char *inner_name;
    char *data;
    int value;
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

Table* create_table(FILE *file, char **error_message);
int create_header(const char *header_line, HeaderCell *header, char delimiter, char **error_message);
void add_row(Table *table, const char *line_str, char delimiter, char **error_message);
Cell* create_cell(const char *name, const char *data);
void free_table(Table *table);
void print_table(Table *table);


#endif // __TABLE__