# UnderBudget source CMake configuration

# Add source sub-directories
add_subdirectory(accounting)
add_subdirectory(budget)
#add_subdirectory(engine)
#add_subdirectory(ledger)
add_subdirectory(ui)

# Instruct CMake to place executable in the bin folder
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

# Configure the application info header
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/info.hpp.in
	${CMAKE_CURRENT_BINARY_DIR}/info.hpp)

# Specify underbudget source files
set(underbudget_srcs
	main.cpp
)

# Specify resource files
# (this can't be done directly in a subdirectory,
# so sub-projects should set their respective _rcs variables)
qt5_add_resources(ui_rcs_srcs ${ui_rcs})

# Build underbudget executable
add_executable(underbudget ${underbudget_srcs} ${ui_rcs_srcs})
target_link_libraries(underbudget ui)
qt5_use_modules(underbudget Core Widgets)

# Add executable to install target
install(PROGRAMS ${PROJECT_BINARY_DIR}/bin/underbudget DESTINATION bin)

