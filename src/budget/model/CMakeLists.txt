# Build budget model library
define_library(budget_model
    Budget.cpp
    Budget.hpp
    Expense.cpp
    Expense.hpp
    Impact.cpp
    Impact.hpp
    Income.cpp
    Income.hpp
    Recurrence.cpp
    Recurrence.hpp
)
qt5_use_modules(budget_model Core)
target_link_libraries(budget_model ledger_model)
