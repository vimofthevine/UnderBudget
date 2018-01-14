# Build ledger model library
define_library(ledger_model
    Account.cpp
    Account.hpp
    AccountTransaction.cpp
    AccountTransaction.hpp
    Currency.cpp
    Currency.hpp
    Envelope.cpp
    Envelope.hpp
    EnvelopeTransaction.cpp
    EnvelopeTransaction.hpp
    JournalEntry.cpp
    JournalEntry.hpp
    Money.cpp
    Money.hpp
    Reconciliation.cpp
    Reconciliation.hpp
    Transaction.cpp
    Transaction.hpp
)
qt5_use_modules(ledger_model Core)
