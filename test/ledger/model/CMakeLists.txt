# Ledger model test CMake configuration

# Build unit tests
build_gtest(LedgerEntryTest ledger_model)
build_test(CurrencyTest ledger_model)
build_test(MoneyTest ledger_model)
