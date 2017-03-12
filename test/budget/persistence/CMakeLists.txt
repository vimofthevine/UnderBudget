# Budget persistence test CMake configuration

# Build unit tests
build_gtest(SQLExpenseRepositoryTest budget_persistence)
build_gtest(SQLIncomeRepositoryTest budget_persistence)
