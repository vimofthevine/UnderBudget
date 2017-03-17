# Budget model source CMake configuration

# Specify budget model source files
set(budget_model_srcs
    Budget.cpp
    Expense.cpp
    Impact.cpp
    Income.cpp
    Recurrence.cpp
)

# Build budget model library
add_library(budget_model ${budget_model_srcs})
qt5_use_modules(budget_model Core)
target_link_libraries(budget_model ledger_model)
