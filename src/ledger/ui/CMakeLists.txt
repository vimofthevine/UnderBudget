# Ledger UI source CMake configuration

# Specify ledger UI source files
set(ledger_ui_srcs
    AccountDetailsDialog.cpp
    AccountModel.cpp
    AccountListWidget.cpp
    AccountSplitModel.cpp
    AccountTransactionModel.cpp
    DoubleLineEdit.cpp
    DoubleValidator.cpp
    EnvelopeDetailsDialog.cpp
    EnvelopeModel.cpp
    EnvelopeListWidget.cpp
    EnvelopeSplitModel.cpp
    EnvelopeTransactionModel.cpp
    JournalEntryDialog.cpp
    TransactionTableView.cpp
    TreeView.cpp
)

# Build ledger UI library
add_library(ledger_ui ${ledger_ui_srcs})
qt5_use_modules(ledger_ui Core Script Widgets)
target_link_libraries(ledger_ui ledger_persistence ledger_model)
