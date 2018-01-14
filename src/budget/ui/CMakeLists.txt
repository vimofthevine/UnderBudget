# Build budget UI library
define_library(budget_ui
    BudgetTableView.cpp
    BudgetTableView.hpp
    ExpenseDetailsDialog.cpp
    ExpenseDetailsDialog.hpp
    ExpenseListWidget.cpp
    ExpenseListWidget.hpp
    ExpenseModel.cpp
    ExpenseModel.hpp
    IncomeDetailsDialog.cpp
    IncomeDetailsDialog.hpp
    IncomeListWidget.cpp
    IncomeListWidget.hpp
    IncomeModel.cpp
    IncomeModel.hpp
    RecurrenceEditWidget.cpp
    RecurrenceEditWidget.hpp
)
qt5_use_modules(budget_ui Core Widgets)
target_link_libraries(budget_ui budget_persistence budget_model ledger_ui)
set_target_properties(budget_ui PROPERTIES AUTOMOC ON)
