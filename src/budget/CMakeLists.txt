# CMake config for UnderBudget budget namespace

# Specify budget source files
set(budget_srcs
	Estimate.cpp
)

# Tell CMake to create budget library
add_library(budget ${budget_srcs})

