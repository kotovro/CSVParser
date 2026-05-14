#include <stddef.h>

#include "asserts.h"

#include "create_header_tests.h"
#include "get_line_number_from_token_tests.h"
#include "get_line_size_tests.h"
#include "try_get_operands_tests.h"

extern int tests_run;
extern int tests_failed;


int main(void) {
    printf("--- Running customized tests ---\n");
    
    // run_safe_strtol_tests();
    run_get_operands_tests();
    run_create_header_tests();
    run_get_line_number_from_token_tests();
    run_get_line_number_from_token_tests();


    printf("\n--- Test Summary ---\n");
    printf("Total tests run: %d\n", tests_run);
    printf("Tests passed:    %d\n", tests_run - tests_failed);
    printf("Tests failed:    %d\n", tests_failed);
    
    return (tests_failed > 0) ? 1 : 0;
}