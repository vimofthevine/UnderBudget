# Budget UI source CMake configuration

# Specify budget UI source files
set(budget_ui_srcs
    BudgetTableView.cpp
    ExpenseDetailsDialog.cpp
    ExpenseListWidget.cpp
    ExpenseModel.cpp
    RecurrenceEditWidget.cpp
)

# Build budget UI library
add_library(budget_ui ${budget_ui_srcs})
qt5_use_modules(budget_ui Core Widgets)
target_link_libraries(budget_ui budget_persistence budget_model ledger_ui)
