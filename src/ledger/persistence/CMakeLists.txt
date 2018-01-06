# Build ledger persistence library
define_library(ledger_persistence
    SQLAccountRepository.cpp
    SQLAccountRepository.hpp
    SQLCurrencyRepository.cpp
    SQLCurrencyRepository.hpp
    SQLEnvelopeRepository.cpp
    SQLEnvelopeRepository.hpp
    SQLReconciliationRepository.cpp
    SQLReconciliationRepository.hpp
    SQLTransactionRepository.cpp
    SQLTransactionRepository.hpp
)
qt5_use_modules(ledger_persistence Core Sql)
target_link_libraries(ledger_persistence ledger_model)
