
#include <ctype.h>
#include <string.h>

#include "errors_utils.h"
#include "parser.h"

Cell* get_cell_from_string(Table* table, char* operand_str);
bool get_value_from_cell(Table* table, Cell* cell, char* cell_name, char** error_message);
bool get_operand_from_string(Table* table, char* operand_str, long* operand_val, char* cell_name, char** error_message);
bool get_value_from_string(Table* table, const char* str, char* cell_name, long* out_val, char** error_message);

void parse_table(Table *table, char **error_message) {
    Line* current_line = table->first_line;
    while (current_line) {
        for (size_t i = 0; i < table->column_count - 1; i++) {
            char *cell_name = get_cell_reference_name(i, current_line->line_number, table->first_header_cell);
            if (current_line->cells[i].value_state == CELL_VALUE_UNPARSED) {
                if (get_value_from_cell(table, &current_line->cells[i], cell_name, error_message)) {
                    free(cell_name);
                } else {
                    free(cell_name);
                    return;
                }
            }
        }
        current_line = current_line->next;
    }
}

bool get_value_from_cell(Table* table, Cell* cell, char* cell_name, char** error_message) {
    if (cell->value_state == CELL_VALUE_PENDING) {
        set_error_message(error_message, "Cyclic dependency detected in cell: %s", cell_name);
        return false;
    }

    cell->value_state = CELL_VALUE_PENDING;

    if (cell->data[0] != '=') {
        if(!safe_strtol(cell->data, &cell->value)) {
            char *message = "Invalid numeric value %s in cell: %s";
            size_t message_len = strlen(message) + strlen(cell->data) + strlen(cell_name) + 1 - 4;
            *error_message = malloc(message_len);
            sprintf(*error_message, message, cell->data, cell_name);
            return false;
        }
        cell->value_state = CELL_VALUE_PARSED;
        return true;
    } 
    if (get_value_from_string(table, cell->data + 1, cell_name, &cell->value, error_message)) {
        cell->value_state = CELL_VALUE_PARSED;
    }
    else {
        return false;
    }
    return true;
}

bool get_value_from_string(Table *table, const char* str, char* cell_name, long* out_val, char** error_message) {
    char operator;

    char *operand1_str, *operand2_str;
    if (!try_get_operands(str, &operand1_str, &operand2_str, &operator)) {
        char *message = "Invalid formula %s in cell: %s";
        size_t message_len = strlen(message) + strlen(str) + strlen(cell_name) + 1 - 4;
        *error_message = malloc(message_len);
        sprintf(*error_message, message, str, cell_name);
        return false; // Invalid formula
    }

    long operand1_val, operand2_val;
    if (!get_operand_from_string(table, operand1_str, &operand1_val, cell_name, error_message) 
        || !get_operand_from_string(table, operand2_str, &operand2_val, cell_name, error_message)) {
        
        free(operand1_str);
        free(operand2_str);
        return false;
    }

    free(operand1_str);
    free(operand2_str);

    if (operator == '+') {
        *out_val = operand1_val + operand2_val;
    } else if (operator == '-') {
        *out_val = operand1_val - operand2_val;
    } else if (operator == '*') {
        *out_val = operand1_val * operand2_val;
    } else if (operator == '/') {
        if (operand2_val == 0) {
            set_error_message(error_message, "Division by zero in cell: %s", cell_name);
            return false; // Division by zero
        }
        *out_val = operand1_val / operand2_val;
    }
    return true;
}


bool get_operand_from_string(Table *table, char* operand_str, long* operand_val, char* cell_name, char **error_message) {
    if (!safe_strtol(operand_str, operand_val)) {
        ///Assune that it's cell name, try to get value from cell
        Cell* cell = get_cell_from_string(table, operand_str);
        if (cell == NULL) {
            char *message = "Invalid cell reference %s in cell %s.";
            size_t message_len = strlen(message) + strlen(operand_str) + strlen(cell_name) + 1 - 4;
            *error_message = malloc(message_len);
            sprintf(*error_message, message, operand_str, cell_name);
            return false;
        }
        if (cell->value_state == CELL_VALUE_PARSED) {
            *operand_val = cell->value;
        } else {
            if (get_value_from_cell(table, cell, operand_str, error_message)) {
                *operand_val = cell->value;
            } else {
                return false;
            }
        }
    }
    return true;
}


Cell* get_cell_from_string(Table* table, char* operand_str) {
    if (!isdigit((unsigned char)operand_str[strlen(operand_str) - 1])) {
        return NULL;
    }
        
    //iterate backwords for getting line number, then, when we get something that can;r pe parsed   
    size_t row_number_pos = strlen(operand_str) - 1;
    
    while (row_number_pos > 0 && isdigit((unsigned char)operand_str[row_number_pos])) {
        --row_number_pos;    
    }

    long row_number;
    if (!safe_strtol(operand_str + row_number_pos + 1, &row_number)) {
        return NULL;
    }
    
    char* column_name = malloc(row_number_pos + 2);
    for (size_t i = 0; i < row_number_pos + 1; ++i) {
        column_name[i] = operand_str[i];
    }
    column_name[row_number_pos + 1] = '\0';

    long col_idx = get_column_index(table->first_header_cell, column_name);
    free(column_name);
    if (col_idx == -1) {
        return NULL;
    }

    return get_cell_by_position(table, col_idx, row_number);
}