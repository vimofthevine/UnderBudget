# Budget source CMake configuration

# Add budget source sub-directories
add_subdirectory(storage)

# Specify budget source files
set(budget_srcs
	AddChildEstimateCommand.cpp
	Budget.cpp
	BudgetingPeriod.cpp
	BudgetingPeriod_calculations.cpp
	ChangeBudgetNameCommand.cpp
	ChangeEstimateAmountCommand.cpp
	ChangeEstimateDescriptionCommand.cpp
	ChangeEstimateDueDateCommand.cpp
	ChangeEstimateFinishedCommand.cpp
	ChangeEstimateNameCommand.cpp
	ChangeEstimateTypeCommand.cpp
	ChangeInitialBalanceCommand.cpp
	ChangePeriodParamsCommand.cpp
	DeleteEstimateCommand.cpp
	Estimate.cpp
)

# Build budget library
add_library(budget ${budget_srcs})
qt5_use_modules(budget Core Widgets)
target_link_libraries(budget accounting)

