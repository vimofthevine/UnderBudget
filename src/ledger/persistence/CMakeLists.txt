# Ledger persistence source CMake configuration

# Specify ledger persistence source files
set(ledger_persistence_srcs
    AssetAccount.cpp
)

# Build ledger persistence library
add_library(ledger_persistence ${ledger_persistence_srcs})
qt5_use_modules(ledger_persistence Core Sql)
