# Budget persistence source CMake configuration

# Specify budget persistence source files
set(budget_persistence_srcs
    SQLExpenseRepository.cpp
    SQLIncomeRepository.cpp
)

# Build budget persistence library
add_library(budget_persistence ${budget_persistence_srcs})
qt5_use_modules(budget_persistence Core Sql)
target_link_libraries(budget_persistence budget_model)
