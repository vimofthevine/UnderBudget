# Ledger persistence test CMake configuration

# Build unit tests
build_gtest(SQLAccountRepositoryTest ledger_persistence)
build_gtest(SQLEnvelopeRepositoryTest ledger_persistence)
build_test(SQLCurrencyRepositoryTest ledger_persistence)
