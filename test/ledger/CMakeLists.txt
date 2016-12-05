# Ledger test CMake configuration

# Build unit tests
build_test(AccountTest ledger)
#build_test(TransactionTest ledger)

# Add subdirectories
add_subdirectory(model)
add_subdirectory(persistence)
add_subdirectory(storage)

