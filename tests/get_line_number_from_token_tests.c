#include "get_line_number_from_token_tests.h"
#include <stdlib.h>

void get_line_number_from_token(const char* token, long *line_number, char** error_message);

void test_valid_line_number(void);
void test_invalid_line_number(void);
void test_empty_line_number(void);

void run_get_line_number_from_token_tests(void) {
    test_valid_line_number();
    test_invalid_line_number();
    test_empty_line_number();
}


void test_valid_line_number(void) {
    long line_number = 0;
    char *error_message = NULL;
    char delimiter = ',';
    char str[] = "1,2";
    char *token = strtok(str, &delimiter);
    get_line_number_from_token(token, &line_number, &error_message);
    ASSERT_PTR_EQ(NULL, error_message);
    ASSERT_NUM_EQ(1, line_number);
}

void test_invalid_line_number(void) {
    long line_number = 0;
    char *error_message = NULL;
    char delimiter = ',';
    char str[] = "abc,2";
    char *token = strtok(str, &delimiter);
    get_line_number_from_token(token, &line_number, &error_message);
    ASSERT_STR_EQ("Invalid line number: abc", error_message);
    free(error_message);
}


void test_empty_line_number(void) {
    long line_number = 0;
    char *error_message = NULL;
    char delimiter = ',';
    char str[] = ",";
    char *token = strtok(str, &delimiter);
    fprintf(stderr, "Token: %s\n", token);
    get_line_number_from_token(token, &line_number, &error_message);
    ASSERT_STR_EQ("Line number token is missing.", error_message);
    free(error_message);
}

