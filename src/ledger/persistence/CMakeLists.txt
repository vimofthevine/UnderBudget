# Ledger persistence source CMake configuration

# Specify ledger persistence source files
set(ledger_persistence_srcs
    SQLAccountRepository.cpp
    SQLCurrencyRepository.cpp
    SQLEnvelopeRepository.cpp
    SQLReconciliationRepository.cpp
    SQLTransactionRepository.cpp
)

# Build ledger persistence library
add_library(ledger_persistence ${ledger_persistence_srcs})
qt5_use_modules(ledger_persistence Core Sql)
target_link_libraries(ledger_persistence ledger_model)
