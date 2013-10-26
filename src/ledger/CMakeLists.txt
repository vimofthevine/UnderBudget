# Ledger source CMake configuration

# Add ledger source sub-directories
#add_subdirectory(storage)

# Specify ledger source files
set(ledger_srcs
	Account.cpp
	#ImmutableTransaction.cpp
	#transactions.cpp
)

# Build ledger library
add_library(ledger ${ledger_srcs})
qt5_use_modules(ledger Core)
target_link_libraries(ledger accounting)

