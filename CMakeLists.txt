cmake_minimum_required(VERSION 3.16)
project(csvparser LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Output directory (mirrors _build/) 
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY         ${CMAKE_CURRENT_LIST_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_LIST_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG   ${CMAKE_CURRENT_LIST_DIR})
file(GLOB SRCS "src/*.c")
add_executable(csvparser ${SRCS})

message(STATUS "Compiler        : ${CMAKE_C_COMPILER_ID}")
message(STATUS "Compiler path   : ${CMAKE_C_COMPILER}")
message(STATUS "Frontend variant: ${CMAKE_C_COMPILER_FRONTEND_VARIANT}")
message(STATUS "Cygwin          : ${CYGWIN}")


#  Priority order (first match wins):
#   1. Cygwin          – GCC running inside the Cygwin POSIX layer
#   2. MSVC  (cl.exe)  – classic Microsoft compiler
#   3. Clang/LLVM
#        a. clang-cl   – Clang with MSVC-compatible frontend (/flag style)
#        b. clang      – Clang with GNU-compatible frontend  (-flag style)
#   4. Everything else (GCC on Linux/macOS)
#
if(CYGWIN)
    # Cygwin sets WIN32 *and* UNIX, so it must be tested before the MSVC block.
    # Flags are identical to native Linux GCC.
    message(STATUS "Toolchain: Cygwin GCC")
    target_compile_options(csvparser PRIVATE
        -Wall
        -Wextra
        -Werror
        -O3
    )

elseif(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
    # 2. cl.exe (MSVC)
    message(STATUS "Toolchain: MSVC (cl.exe)")
    target_compile_options(csvparser PRIVATE
        /W4         # ~= -Wall -Wextra
        /WX         # warnings as errors (-Werror)
        /O2         # closest MSVC equivalent to -O3
        /MP         # parallel compilation (free speed-up)
    )
    target_compile_definitions(csvparser PRIVATE
        _CRT_SECURE_NO_WARNINGS   # silence "use _s variants" noise
    )
    target_link_libraries(csvparser PRIVATE ws2_32)

elseif(CMAKE_C_COMPILER_ID STREQUAL "Clang")

    if(CMAKE_C_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
        #3a. clang-cl (LLVM, MSVC-compatible frontend)
        # Invoked as:  cmake -DCMAKE_C_COMPILER=clang-cl ...
        # Driver accepts /flags; raw Clang flags go through /clang:<flag>
        message(STATUS "Toolchain: clang-cl (LLVM + MSVC frontend)")
        target_compile_options(csvparser PRIVATE
            /W4                   # MSVC-driver warning level
            /WX                   # warnings as errors
            /O2                   # optimise
            /clang:-Wextra        # pass -Wextra straight to the Clang front-end
            /clang:-Wno-unused-command-line-argument
        )
        target_compile_definitions(csvparser PRIVATE
            _CRT_SECURE_NO_WARNINGS
        )
        target_link_libraries(csvparser PRIVATE ws2_32)

    else()
        # ── 3b. clang (LLVM, GNU-compatible frontend) ─────────────────────────
        # Invoked as:  cmake -DCMAKE_C_COMPILER=clang ...
        # Works on Windows (MSYS2/winlibs), Linux, and macOS.
        message(STATUS "Toolchain: Clang (LLVM + GNU frontend)")
        target_compile_options(csvparser PRIVATE
            -Wall
            -Wextra
            -Werror
            -O3
        )
        if(WIN32)
            # Native Windows clang (not Cygwin) still needs the socket lib
            target_link_libraries(csvparser PRIVATE ws2_32)
            target_compile_definitions(csvparser PRIVATE _CRT_SECURE_NO_WARNINGS)
        endif()
    endif()

else()
    # ── 4. GCC (Linux / macOS / MinGW) ───────────────────────────────────────
    message(STATUS "Toolchain: GCC (or compatible)")
    target_compile_options(csvparser PRIVATE
        -Wall
        -Wextra
        -Werror
        -O3
    )
    if(WIN32)
        # MinGW-w64 GCC on Windows
        target_link_libraries(csvparser PRIVATE ws2_32)
        target_compile_definitions(csvparser PRIVATE _CRT_SECURE_NO_WARNINGS)
    endif()
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
# cmake --build <build_dir> --target clean
# or:   cmake -E rm -rf <build_dir>