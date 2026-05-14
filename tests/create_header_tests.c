#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "asserts.h"
#include "../src/table.h"
#include "create_header_tests.h"

void free_header(HeaderCell *header);

void test_multiple_columns(void);
void test_different_delimiter(void);
void test_missing_leading_delimiter(void);
void test_missing_leading_delimiter_semicolon(void);
void test_invalid_column_name(void);
void test_duplicate_column_name(void);
void test_trailing_delimiter_causes_empty_column(void);
void test_return_value_matches_column_count(void);

void run_create_header_tests(void) {
    test_multiple_columns();
    test_different_delimiter();
    test_missing_leading_delimiter();
    test_missing_leading_delimiter_semicolon();
    test_invalid_column_name();
    test_duplicate_column_name();
    test_trailing_delimiter_causes_empty_column();
    test_return_value_matches_column_count();
}

void test_multiple_columns(void) {
    HeaderCell* root = malloc(sizeof(HeaderCell));
    root->next = NULL;
    root->column_name = NULL;

    char *err = NULL;
    int result = create_header(",col1a,col2q,coll", root, ',', &err);

    ASSERT_NUM_EQ(4, result);
    ASSERT_PTR_EQ(err, NULL);
    ASSERT_STR_EQ("col1a", root->column_name);
    ASSERT_STR_EQ("col2q", root->next->column_name);
    ASSERT_STR_EQ("coll", root->next->next->column_name);
    ASSERT_PTR_EQ(root->next->next->next, NULL);

    free_header(root);
}

void test_different_delimiter(void) {
    HeaderCell *root = malloc(sizeof(HeaderCell));
    root->next = NULL;
    root->column_name = NULL;
    char *err = NULL;
    int result = create_header(";cola;colb", root, ';', &err);

    ASSERT_NUM_EQ(3, result);
    ASSERT_PTR_EQ(err, NULL);
    ASSERT_STR_EQ("cola", root->column_name);
    ASSERT_STR_EQ("colb", root->next->column_name);

    free_header(root);
}

void test_missing_leading_delimiter(void) {
    HeaderCell *root = malloc(sizeof(HeaderCell));
    root->next = NULL;
    root->column_name = NULL;
    char *err = NULL;
    int result = create_header("col1,col2", root, ',', &err);

    ASSERT_NUM_EQ(-1, result);
    ASSERT_STR_EQ(err, "First column name should be empty.");

    free(err);
    free_header(root);
}

void test_missing_leading_delimiter_semicolon(void) {
    HeaderCell *root = malloc(sizeof(HeaderCell));
    root->next = NULL;
    root->column_name = NULL;
    char *err = NULL;
    int result = create_header("colh;colj", root, ';', &err);

    ASSERT_NUM_EQ(-1, result);
    ASSERT_STR_EQ(err, "First column name should be empty.");
    
    free(err);
    free_header(root);
}

void test_invalid_column_name(void) {
    HeaderCell *root = malloc(sizeof(HeaderCell));
    root->next = NULL;
    root->column_name = NULL;
    char *err = NULL;
    int result = create_header(",col1", root, ',', &err);

    ASSERT_NUM_EQ(-1, result);
    ASSERT_STR_EQ("Invalid column name: col1", err);

    free(err);
    free_header(root);
}

void test_duplicate_column_name(void) {
    HeaderCell *root = malloc(sizeof(HeaderCell));
    root->next = NULL;
    root->column_name = NULL;
    char *err = NULL;
    int result = create_header(",cola,cola,colb", root, ',', &err);

    ASSERT_NUM_EQ(-1, result);
    ASSERT_STR_EQ(err, "Duplicate column name: cola");

    free(err);
    free_header(root);
}

void test_trailing_delimiter_causes_empty_column(void) {
    HeaderCell *root = malloc(sizeof(HeaderCell));
    root->next = NULL;
    root->column_name = NULL;
    char *err = NULL;
    int result = create_header(",colb,colf,", root, ',', &err);

    ASSERT_NUM_EQ(-1, result);
    ASSERT_STR_EQ(err, "Too many empty column names.");

    free(err);
    free_header(root);
}

void test_only_delimiters(void) {
    HeaderCell *root = malloc(sizeof(HeaderCell));
    root->next = NULL;
    root->column_name = NULL;
    char *err = NULL;
    int result = create_header(",,,", root, ',', &err);

    ASSERT_NUM_EQ(-1, result);
    ASSERT_STR_EQ(err, "Too many empty column names.");

    free(err);
    free_header(root);
}

void test_return_value_matches_column_count(void) {
    HeaderCell *root = malloc(sizeof(HeaderCell));
    root->next = NULL;
    root->column_name = NULL;
    char *err = NULL;
    int result = create_header(",a,b,c,d,e", root, ',', &err);

    ASSERT_NUM_EQ(6, result);
    ASSERT_PTR_EQ(err, NULL);

    free_header(root);
}