cmake_minimum_required(VERSION 3.16)
project(csvparser LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Output directory (mirrors _build/)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_LIST_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_LIST_DIR})

file(GLOB SRCS "src/*.c")
file(GLOB TEST_SRCS "tests/*.c")  

# ── Helper: apply compiler flags to a target ─────────────────────────────────
# Extracted into a function so both executables share identical flags without
# duplicating the entire if/elseif chain.
function(apply_compiler_options TARGET)
    if(CYGWIN)
        message(STATUS "Toolchain: Cygwin GCC")
        target_compile_options(${TARGET} PRIVATE -Wall -Wextra -Werror -O3)

    elseif(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
        message(STATUS "Toolchain: MSVC (cl.exe)")
        target_compile_options(${TARGET} PRIVATE /W4 /WX /O2 /MP)
        target_compile_definitions(${TARGET} PRIVATE _CRT_SECURE_NO_WARNINGS)
        target_link_libraries(${TARGET} PRIVATE ws2_32)

    elseif(CMAKE_C_COMPILER_ID STREQUAL "Clang")
        if(CMAKE_C_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
            message(STATUS "Toolchain: clang-cl (LLVM + MSVC frontend)")
            target_compile_options(${TARGET} PRIVATE
                /W4 /WX /O2
                /clang:-Wextra
                /clang:-Wno-unused-command-line-argument
            )
            target_compile_definitions(${TARGET} PRIVATE _CRT_SECURE_NO_WARNINGS)
            target_link_libraries(${TARGET} PRIVATE ws2_32)
        else()
            message(STATUS "Toolchain: Clang (LLVM + GNU frontend)")
            target_compile_options(${TARGET} PRIVATE -Wall -Wextra -Werror -O3)
            if(WIN32)
                target_link_libraries(${TARGET} PRIVATE ws2_32)
                target_compile_definitions(${TARGET} PRIVATE _CRT_SECURE_NO_WARNINGS)
            endif()
        endif()

    else()
        message(STATUS "Toolchain: GCC (or compatible)")
        target_compile_options(${TARGET} PRIVATE -Wall -Wextra -Werror -O3)
        if(WIN32)
            target_link_libraries(${TARGET} PRIVATE ws2_32)
            target_compile_definitions(${TARGET} PRIVATE _CRT_SECURE_NO_WARNINGS)
        endif()
    endif()
endfunction()

# ── Diagnostics ───────────────────────────────────────────────────────────────
message(STATUS "Compiler:          ${CMAKE_C_COMPILER_ID}")
message(STATUS "Compiler path:     ${CMAKE_C_COMPILER}")
message(STATUS "Frontend variant:  ${CMAKE_C_COMPILER_FRONTEND_VARIANT}")
message(STATUS "Cygwin:            ${CYGWIN}")


add_executable(csvparser main.c ${SRCS})
apply_compiler_options(csvparser)

if(TEST_SRCS)
    add_executable(run_tests ${TEST_SRCS} ${SRCS})
    apply_compiler_options(run_tests)

    add_custom_target(csvparser_tests
        COMMAND run_tests
        DEPENDS run_tests
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
        COMMENT "Running tests..."
    )
else()
    message(STATUS "No test sources found in test/ — skipping test target")
endif()

if(WIN32 AND NOT CYGWIN)
    add_custom_target(clear
        COMMAND cmd /c rmdir /s /q "${CMAKE_BINARY_DIR}"
        COMMENT "Deleting entire build directory..."
    )
else()
    add_custom_target(clear
        COMMAND rm -rf "${CMAKE_BINARY_DIR}"
        COMMENT "Deleting entire build directory..."
    )
endif()