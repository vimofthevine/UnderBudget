#Ledger persistence test CMake configuration

#Build unit tests
build_gtest(SQLAccountRepositoryTest ledger_persistence)
    build_gtest(SQLCurrencyRepositoryTest ledger_persistence)
        build_gtest(SQLEnvelopeRepositoryTest ledger_persistence)
            build_gtest(SQLReconciliationRepositoryTest ledger_persistence)
                build_gtest(SQLTransactionRepositoryTest ledger_persistence)
