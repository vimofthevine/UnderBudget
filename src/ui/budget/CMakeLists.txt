# UI budget source CMake configuration

# Specify UI budget source files
set(ui_budget_srcs
	AssignmentRulesModel.cpp
	BudgetDetailsForm.cpp
	BudgetingPeriodForm.cpp
	ConditionFieldDelegate.cpp
	ConditionOperatorDelegate.cpp
	EstimateDetailsForm.cpp
	EstimateDisplayWidget.cpp
	EstimateModel.cpp
	EstimateTreeWidget.cpp
	RuleAddProxyCommand.cpp
	RuleChangeProxyCommand.cpp
	RuleRemoveProxyCommand.cpp
	ProgressDelegate.cpp
	ProxyModelAddCommand.cpp
	ProxyModelChangeCommand.cpp
	ProxyModelDeleteCommand.cpp
	ProxyModelMoveCommand.cpp
	RulesListWidget.cpp
)

# Build UI budget library
add_library(ui_budget ${ui_budget_srcs})
qt5_use_modules(ui_budget Core Widgets)
target_link_libraries(ui_budget budget ui_accounting)

