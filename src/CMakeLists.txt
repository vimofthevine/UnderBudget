# CMake config for UnderBudget
cmake_minimum_required(VERSION 2.8.9)
project(UnderBudget)

# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Have to include the binary directory to include the
# moc-generated files
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Find the QtWidgets library
find_package(QtWidgets)

# Include source sub-directories
add_subdirectory(budget)
add_subdirectory(engine)
add_subdirectory(finance)
add_subdirectory(ledger)
add_subdirectory(ui)

# Specify main source files
set(main_srcs
	main.cpp
)

# Tell CMake to create the underbudget executable
add_executable(underbudget ${main_srcs})
qt5_use_modules(underbudget Widgets)

