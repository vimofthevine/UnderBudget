# UnderBudget source CMake configuration

# Add source sub-directories
add_subdirectory(accounting)
#add_subdirectory(budget)
#add_subdirectory(engine)
#add_subdirectory(ledger)
#add_subdirectory(ui)

# Instruct CMake to place executable in the bin folder
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

# Specify underbudget source files
set(underbudget_srcs
	main.cpp
)

# Build underbudget executable
#add_executable(underbudget ${underbudget_srcs})
#qt5_use_modules(underbudget Core Widgets)

# Add executable to install target
#install(PROGRAMS ${PROJECT_BINARY_DIR}/bin/underbudget DESTINATION bin)

