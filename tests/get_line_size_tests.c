#include <stdlib.h>

#include "asserts.h"
#include "get_line_size_tests.h"


void check_row_size(const char* line_str, char delimiter, size_t expected_columns, char** error_message);

void test_valid_row_size(void);
void test_invalid_row_size(void);


void run_get_line_size_tests(void) {
    test_valid_row_size();
    test_invalid_row_size();
}

void test_valid_row_size(void) {
    char *error_message = NULL;
    check_row_size("1,2,3", ',', 3, &error_message);
    ASSERT_PTR_EQ(NULL, error_message);
}

void test_invalid_row_size(void) {
    char *error_message = NULL;
    check_row_size("1,2,3", ',', 4, &error_message);
    ASSERT_STR_EQ("Incorrect number of cells in line: 1,2,3", error_message);
    free(error_message);
}
