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
    bool is_valid;
    Cell *next;
};

struct HeaderCell {
    HeaderCell *next;
    char *column_name;
};


struct Line {
    Cell *cells;
    bool is_valid;
    int number;
};

struct Table {
    HeaderCell *header;
    Line *lines;
    size_t line_count;
    size_t column_count;
};

Table* create_table(FILE *file, char **error_message);
int create_header(const char *header_line, HeaderCell *header, char delimiter, char **error_message);
Cell* create_cell(const char *name, const char *data);
void free_table(Table *table);

#endif // __TABLE__