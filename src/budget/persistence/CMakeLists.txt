# Build budget persistence library
define_library(budget_persistence
    SQLExpenseRepository.cpp
    SQLExpenseRepository.hpp
    SQLIncomeRepository.cpp
    SQLIncomeRepository.hpp
)
qt5_use_modules(budget_persistence Core Sql)
target_link_libraries(budget_persistence budget_model)
