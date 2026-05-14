#include <stdlib.h>
#include "asserts.h"
#include "../src/parserutils.h"
#include "try_get_operands_tests.h"

void test_simple_addition(void);
void test_simple_subtraction(void);
void test_multiplication(void);
void test_division(void);
void test_invalid_formula_returns_false(void);
void test_empty_string_returns_false(void);
void test_null_returns_false(void);
void test_single_digit_operands(void);
void test_large_numbers(void);


void run_get_operands_tests(void) {
    test_simple_addition();
    test_simple_subtraction();
    test_multiplication();
    test_division();
    test_invalid_formula_returns_false();
    test_empty_string_returns_false();
    test_null_returns_false();
    test_single_digit_operands();
    test_large_numbers();
}


void test_simple_addition(void) {
    char *op1 = NULL, *op2 = NULL;
    char op;
    bool result = try_get_operands("12+34", &op1, &op2, &op);
    ASSERT_BOOL_EQ(result, true);
    ASSERT_STR_EQ("12", op1);
    ASSERT_STR_EQ("34", op2);
    ASSERT_NUM_EQ('+', op);
    free(op1); free(op2);
}

void test_simple_subtraction(void) {
    char *op1 = NULL, *op2 = NULL;
    char op;
    bool result = try_get_operands("100-1", &op1, &op2, &op);
    ASSERT_BOOL_EQ(result, true);
    ASSERT_STR_EQ("100", op1);
    ASSERT_STR_EQ("1", op2);
    ASSERT_NUM_EQ('-', op);
    free(op1); free(op2);
}

void test_multiplication(void) {
    char *op1 = NULL, *op2 = NULL;
    char op;
    bool result = try_get_operands("6*7", &op1, &op2, &op);
    ASSERT_BOOL_EQ(result, true);
    ASSERT_STR_EQ("6", op1);
    ASSERT_STR_EQ("7", op2);
    ASSERT_NUM_EQ('*', op);
    free(op1); free(op2);
}

void test_division(void) {
    char *op1 = NULL, *op2 = NULL;
    char op;
    bool result = try_get_operands("10/2", &op1, &op2, &op);
    ASSERT_BOOL_EQ(result, true);
    ASSERT_STR_EQ("10", op1);
    ASSERT_STR_EQ("2", op2);
    ASSERT_NUM_EQ('/', op);
    free(op1); free(op2);
}

void test_invalid_formula_returns_false(void) {
    char *op1 = NULL, *op2 = NULL;
    char op = 0;
    bool result = try_get_operands("abc", &op1, &op2, &op);
    ASSERT_BOOL_EQ(result, false);
    ASSERT_PTR_EQ(op1, NULL);
    ASSERT_PTR_EQ(op2, NULL);
}

void test_empty_string_returns_false(void) {
    char *op1 = NULL, *op2 = NULL;
    char op = 0;
    bool result = try_get_operands("", &op1, &op2, &op);
    ASSERT_BOOL_EQ(result, false);
    ASSERT_PTR_EQ(op1, NULL);
    ASSERT_PTR_EQ(op2, NULL);
}

void test_null_returns_false(void) {
    char *op1 = NULL, *op2 = NULL;
    char op = 0;
    bool result = try_get_operands(NULL, &op1, &op2, &op);
    ASSERT_BOOL_EQ(result, false);
    ASSERT_PTR_EQ(op1, NULL);
    ASSERT_PTR_EQ(op2, NULL);
}

void test_single_digit_operands(void) {
    char *op1, *op2;
    char op;
    bool result = try_get_operands("1+1", &op1, &op2, &op);
    ASSERT_BOOL_EQ(result, true);
    ASSERT_STR_EQ("1", op1);
    ASSERT_STR_EQ("1", op2);
    ASSERT_NUM_EQ('+', op);
    free(op1); free(op2);
}

void test_large_numbers(void) {
    char *op1, *op2;
    char op;
    bool result = try_get_operands("99999+11111", &op1, &op2, &op);
    ASSERT_BOOL_EQ(result, true);
    ASSERT_STR_EQ("99999", op1);
    ASSERT_STR_EQ("11111", op2);
    ASSERT_NUM_EQ('+', op);
    free(op1); free(op2);
}