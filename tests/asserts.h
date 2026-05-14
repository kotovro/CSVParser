#ifndef ASSERTS_H
#define ASSERTS_H

#include <stdio.h>
#include <string.h>

// Глобальные счетчики для статистики тестов
extern int tests_run;
extern int tests_failed;

// Макрос для проверки целых чисел (int, long)
#define ASSERT_NUM_EQ(expected, actual) do { \
    tests_run++; \
    if ((expected) != (actual)) { \
        printf("[FAIL] %s:%d: Expected %ld, but got %ld\n", \
               __FILE__, __LINE__, \
               (long)(expected), (long)(actual)); \
        tests_failed++; \
        return; \
    } \
} while(0)

// Макрос для проверки указателей (NULL / not NULL)
#define ASSERT_PTR_EQ(expected, actual) do { \
    tests_run++; \
    if ((expected) != (actual)) { \
        printf("[FAIL] %s:%d: Expected pointer %p, but got %p\n", \
               __FILE__, __LINE__, (void*)(expected), (void*)(actual)); \
        tests_failed++; \
        return; \
    } \
} while(0)

// Макрос для проверки строк
#define ASSERT_STR_EQ(expected, actual) do { \
    tests_run++; \
    if (strcmp((expected), (actual)) != 0) { \
        printf("[FAIL] %s:%d: Expected \"%s\", but got \"%s\"\n", \
               __FILE__, __LINE__, (expected), (actual)); \
        tests_failed++; \
        return; \
    } \
} while(0)

#define ASSERT_BOOL_EQ(expected, actual) do { \
    tests_run++; \
    if ((expected) != (actual)) { \
        printf("[FAIL] %s:%d: Expected %d, but got %d\n", \
               __FILE__, __LINE__, \
               (bool)(expected), (bool)(actual)); \
        tests_failed++; \
        return; \
    } \
} while(0)


#endif // ASSERTS_H