# Budget test CMake configuration

# Build unit tests
build_test(AssignmentRuleTest budget)
build_test(AssignmentRulesTest budget)
build_test(BalanceTest budget)
build_test(BudgetingPeriodTest budget)
build_test(ConditionsTest budget)
build_test(EstimateTest budget)

# Add test sub-directories
add_subdirectory(storage)

