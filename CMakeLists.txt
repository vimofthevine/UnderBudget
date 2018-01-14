# Project CMake configuration
cmake_minimum_required(VERSION 3.9)
project(UnderBudget VERSION 4.0.0)
set(PROJECT_VERSION 4.0.0-beta)

# Have to include the binary dir for the moc-generated files
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Determine application version from git
if ((NOT DEFINED USE_GIT_VERSION) OR USE_GIT_VERSION)
    message("Getting application version from git...")
    exec_program(
        "git"
        ${CMAKE_CURRENT_SOURCE_DIR}
        ARGS "describe --always --dirty"
        OUTPUT_VARIABLE PROJECT_VERSION
        )
endif ((NOT DEFINED USE_GIT_VERSION) OR USE_GIT_VERSION)
set(APP_VERSION ${PROJECT_VERSION})

add_subdirectory(src)
add_subdirectory(doc)
add_subdirectory(packaging)
if (NOT SKIP_TESTS)
    enable_testing()
    add_subdirectory(test)
endif (NOT SKIP_TESTS)
