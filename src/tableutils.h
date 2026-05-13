#ifndef __TABLEUTILS__
#define __TABLEUTILS__

#include "table.h"

long get_column_index(HeaderCell *first_header_cell, const char *column_name);
Cell* get_cell_by_position(Table *table, long column_index, long line_number);
char* get_cell_reference_name(size_t column_index, size_t line_number, HeaderCell *first_header_cell);

#endif // __TABLEUTILS__