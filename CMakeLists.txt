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
endif (Qt5_POSITION_INDEPENDENT_CODE)

# Enable gcov code coverage in debug mode (only gcc)
if (CMAKE_BUILD_TYPE MATCHES Debug)
	if (CMAKE_COMPILER_IS_GNUCXX)
		message("Enabling debug symbols")
		add_compile_options(-g -O0)
		if (ENABLE_GCOV)
			message("Enabling gcov code coverage")
			add_compile_options(-fprofile-arcs -ftest-coverage)
		endif (ENABLE_GCOV)
	endif (CMAKE_COMPILER_IS_GNUCXX)
endif (CMAKE_BUILD_TYPE MATCHES Debug)

# So all includes will be relative to the src folder
include_directories(src)

# Find the Qt libraries
find_package(Qt5Widgets REQUIRED)

# Determine application version
if (NOT DEFINED USE_GIT_VERSION)
	set(USE_GIT_VERSION true)
endif (NOT DEFINED USE_GIT_VERSION)
if (USE_GIT_VERSION)
	message("Getting application version from git...")
	exec_program(
		"git"
		${CMAKE_CURRENT_SOURCE_DIR}
		ARGS "describe --always"
		OUTPUT_VARIABLE APP_VERSION
		)
else (USE_GIT_VERSION)
	set(APP_VERSION "4.0-beta")
	message("Using hardcoded ${APP_VERSION} as application version")
endif (USE_GIT_VERSION)

# Application source
add_subdirectory(src)
# Unit tests
add_subdirectory(test)
# Documentation
add_subdirectory(doc)
# Packaging
add_subdirectory(packaging)
