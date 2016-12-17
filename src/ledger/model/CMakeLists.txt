# Ledger model source CMake configuration

# Specify ledger model source files
set(ledger_model_srcs
    Currency.cpp
    Money.cpp
)

# Build ledger model library
add_library(ledger_model ${ledger_model_srcs})
qt5_use_modules(ledger_model Core)
