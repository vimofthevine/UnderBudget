# Ledger storage source CMake configuration

# Specify ledger storage source files
set(ledger_storage_srcs
	GnuCashFile.cpp
	GnuCashReader.cpp
	ImportedTransactionSource.cpp
)

# Build ledger storage library
add_library(ledger_storage ${ledger_storage_srcs})
qt5_use_modules(ledger_storage Core)
target_link_libraries(ledger_storage ledger)
target_link_libraries(ledger_storage gzip_support)

