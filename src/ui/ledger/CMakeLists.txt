# UI ledger source CMake configuration

# Specify UI ledger source files
set(ui_ledger_srcs
	ImportedTransactionsListWidget.cpp
	ImportedTransactionsModel.cpp
	MoneyAwareSortFilterProxyModel.cpp
)

# Build UI budget library
add_library(ui_ledger ${ui_ledger_srcs})
qt5_use_modules(ui_ledger Core Widgets)
target_link_libraries(ui_ledger ledger)

