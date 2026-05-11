
#include <string.h>

#include "parser.h"

void parse_table(Table *table, char **error_message) {
    Line* current_line = table->first_line;
    while (current_line) {
        for (size_t i = 0; i < table->column_count - 1; i++) {
            char *cell_name = get_cell_reference_name(i, current_line->line_number, table->first_header_cell);
            if (current_line->cells[i].value_state == CELL_VALUE_UNPARSED) {
                long value;
                if (get_value_from_cell(/*table,*/ &current_line->cells[i], cell_name, &value, error_message)) {
                    current_line->cells[i].value = value;
                    current_line->cells[i].value_state = CELL_VALUE_PARSED;
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

bool get_value_from_cell(/*Table *table,*/ Cell *cell, char *cell_name, long *out_val, char **error_message) {
    if (cell->value_state == CELL_VALUE_PENDING) {
        char *message = "Cyclic dependency detected in cell: ";
        *error_message = malloc(strlen(message) + strlen(cell_name) + 1);
        sprintf(*error_message, "%s%s", message, cell_name);
        
        return false;
    }

    cell->value_state = CELL_VALUE_PENDING;

    if (cell->data[0] != '=') {
        if(!safe_strtol(cell->data, out_val)) {
            char *message = "Invalid numeric value %s in cell: %s";
            size_t message_len = strlen(message) + strlen(cell->data) + strlen(cell_name) + 1 - 4;
            *error_message = malloc(message_len);
            sprintf(*error_message, message, cell->data, cell_name);
            return false;
        }
        return true;
    } 
    return get_value_from_string(/*table, */ cell->data + 1, cell_name, out_val, error_message);
}

bool get_value_from_string(/*Table *table, */const char *str, char *cell_name, long *out_val, char **error_message) {
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
    if (!safe_strtol(operand1_str, &operand1_val)) {
        ///Assune that it's cell name, try to get value from cell
        return false; // Invalid numeric value
    }

    if (!safe_strtol(operand2_str, &operand2_val)) {
        ///Assune that it's cell name, try to get value from cell
        return false; // Invalid numeric value
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
            char *message = "Division by zero in cell: %s";
            size_t message_len = strlen(message) + strlen(cell_name) + 1;
            *error_message = malloc(message_len);
            sprintf(*error_message, message, cell_name);
            return false; // Division by zero
        }
        *out_val = operand1_val / operand2_val;
    }
    return true;
}
