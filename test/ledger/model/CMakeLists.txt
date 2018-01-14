# Build ledger model unit tests
build_gtest(JournalEntryTest ledger_model)
build_test(CurrencyTest ledger_model)
build_test(MoneyTest ledger_model)
