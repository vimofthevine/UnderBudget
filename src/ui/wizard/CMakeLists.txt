# UI wizard source CMake configuration

# Specify UI wizard source files
set(ui_wizard_srcs
	BudgetSourceWizard.cpp
	TransactionSourceWizard.cpp
)

# Build UI wizard library
add_library(ui_wizard ${ui_wizard_srcs})
target_link_libraries(ui_wizard budget_storage)
target_link_libraries(ui_wizard ledger_storage)
qt5_use_modules(ui_wizard Core Widgets)

