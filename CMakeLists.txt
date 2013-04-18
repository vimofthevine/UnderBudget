# Project CMake configuration
cmake_minimum_required(VERSION 2.8.10)
project(UnderBudget)

# Use CTest
enable_testing()

# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Have to include the binary dir for the moc-generated files
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Set -fPIC if needed
if (Qt5_POSITION_INDEPENDENT_CODE)
  set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

# So all includes will be relative to the src folder
include_directories(src)

# Find the Qt libraries
find_package(Qt5Widgets REQUIRED)

# Determine application version
exec_program(
	"git"
	${CMAKE_CURRENT_SOURCE_DIR}
	ARGS "describe --always"
	OUTPUT_VARIABLE APP_VERSION
	)

# Application source
add_subdirectory(src)
# Unit tests
add_subdirectory(test)
# Documentation
add_subdirectory(doc)
