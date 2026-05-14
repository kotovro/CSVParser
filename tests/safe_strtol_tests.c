#include "safe_strtol_tests.h"
#include "asserts.h"
#include "../src/parserutils.h"

void test_valid_number(void);
void test_invalid_number(void);
void test_empty_string(void);
void test_null_string(void);    

void run_safe_strtol_tests(void) {
    test_valid_number();
    test_invalid_number();
    test_empty_string();
    test_null_string();
}

void test_valid_number(void) {
    long out_val;
    bool result = safe_strtol("12345", &out_val);
    ASSERT_BOOL_EQ(true, result);
    ASSERT_NUM_EQ(12345, out_val);
}

void test_invalid_number(void) {
    long out_val;
    bool result = safe_strtol("abc", &out_val);
    ASSERT_BOOL_EQ(false, result);
}

void test_empty_string(void) {
    long out_val;
    bool result = safe_strtol("", &out_val);
    ASSERT_BOOL_EQ(false, result);
}

void test_null_string(void) {
    long out_val;
    bool result = safe_strtol(NULL, &out_val);
    ASSERT_BOOL_EQ(false, result);
}