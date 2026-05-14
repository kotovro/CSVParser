#include <stddef.h>

#include "asserts.h"
#include "try_get_operands_tests.h"
#include "create_header_tests.h"


extern int tests_run;
extern int tests_failed;
// Симулируем функции проекта
// long safe_strtol_parse_success(void) { return safe_strtol("99", NULL, 10); }
// char* get_status() { return "Active"; }
// void* allocate_buffer() { return NULL; } // Намеренная ошибка для демонстрации




// void test_safe_strtol(void) {
//     long result;
//     bool success;
//     // success = 
//     // safe_strtol("123\0", &result);
//     // ASSERT_NUM_EQ(123L, result);
//     // ASSERT_BOOL_EQ(true, success);

//     // success = safe_strtol("abc\0", &result);
//     // ASSERT_BOOL_EQ(false, success);

//     // success = safe_strtol("\0", &result);
//     // ASSERT_BOOL_EQ(false, success);

//     // success = safe_strtol("\01238", &result);
//     // ASSERT_BOOL_EQ(true, success);
//     // Строки
//     // ASSERT_STR_EQ("Active", get_status());
// }

int main(void) {
    printf("--- Running customized tests ---\n");
    
    // test_safe_strtol();
    // test_create_header();

    run_get_operands_tests();
    run_create_header_tests();
    
    // test_only_delimiters();
    // test_return_value_matches_column_count();

    printf("\n--- Test Summary ---\n");
    printf("Total tests run: %d\n", tests_run);
    printf("Tests passed:    %d\n", tests_run - tests_failed);
    printf("Tests failed:    %d\n", tests_failed);
    
    return (tests_failed > 0) ? 1 : 0;
}