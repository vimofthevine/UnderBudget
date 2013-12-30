# Analysis source CMake configuration

# Specify analysis source files
set(analysis_srcs
	Actuals.cpp
	Assignments.cpp
	TransactionAssigner.cpp
)

# Build analysis library
add_library(analysis ${analysis_srcs})
qt5_use_modules(analysis Core)
target_link_libraries(analysis accounting)
target_link_libraries(analysis budget)
target_link_libraries(analysis ledger)

