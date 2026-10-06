cmake_minimum_required(VERSION 3.14)
project(iot_monitor VERSION 1.0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SANITIZER "none" CACHE STRING "Sanitizer (none|asan|ubsan|asan+ubsan)")

# Gate 1: clang-tidy integration
find_program(CLANG_TIDY_EXE NAMES clang-tidy)
if(CLANG_TIDY_EXE)
    set(CMAKE_C_CLANG_TIDY ${CLANG_TIDY_EXE}
        --header-filter=${CMAKE_SOURCE_DIR}/components/.*)
    message(STATUS "[Quality Gate] clang-tidy: ENABLED")
endif()

# Gate 2: cppcheck custom target
find_program(CPPCHECK_EXE NAMES cppcheck)
if(CPPCHECK_EXE)
    add_custom_target(cppcheck
        COMMAND ${CPPCHECK_EXE}
            --check-level=exhaustive
            --enable=all --inconclusive --std=c11
            --suppress=missingIncludeSystem
            --suppress=unusedFunction
            --project=${CMAKE_BINARY_DIR}/compile_commands.json
            -i${CMAKE_BINARY_DIR}/_deps/
            --error-exitcode=1
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "[Quality Gate] Running cppcheck..."
    )
endif()

# Common compile options
add_compile_options(-Wall -Wextra -pedantic -Werror -std=c11 -D_POSIX_C_SOURCE=200809L)

# Sanitizer flags
if(SANITIZER STREQUAL "asan+ubsan")
    add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
    add_link_options(-fsanitize=address,undefined)
endif()

# Components
add_subdirectory(components)

# Main executable
add_subdirectory(app)

# Tests
enable_testing()
add_subdirectory(test)