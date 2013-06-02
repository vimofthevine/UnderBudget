# Budget source CMake configuration

# Add budget source sub-directories
add_subdirectory(storage)

# Specify budget source files
set(budget_srcs
	Budget.cpp
	BudgetingPeriod.cpp
	BudgetingPeriod_calculations.cpp
	ChangeBudgetNameCommand.cpp
	ChangePeriodParamsCommand.cpp
)

# Build budget library
add_library(budget ${budget_srcs})
qt5_use_modules(budget Core Widgets)

