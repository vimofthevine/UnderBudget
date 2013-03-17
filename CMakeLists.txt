# Project CMake configuration
cmake_minimum_required(VERSION 2.8.9)
project(UnderBudget)

# Use CTest
enable_testing()

# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Have to include the binary dir for the moc-generated files
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# So all includes will be relative to the src folder
include_directories(src)

# Find the Qt libraries
find_package(Qt5Widgets REQUIRED)

# Application source
add_subdirectory(src)
# Unit tests
add_subdirectory(test)
