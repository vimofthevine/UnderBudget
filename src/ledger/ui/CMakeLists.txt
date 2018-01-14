# Build ledger UI library
define_library(ledger_ui
    AccountDetailsDialog.cpp
    AccountDetailsDialog.hpp
    AccountModel.cpp
    AccountModel.hpp
    AccountListWidget.cpp
    AccountListWidget.hpp
    AccountSplitModel.cpp
    AccountSplitModel.hpp
    AccountTransactionModel.cpp
    AccountTransactionModel.hpp
    DoubleLineEdit.cpp
    DoubleLineEdit.hpp
    DoubleValidator.cpp
    DoubleValidator.hpp
    EnvelopeDetailsDialog.cpp
    EnvelopeDetailsDialog.hpp
    EnvelopeModel.cpp
    EnvelopeModel.hpp
    EnvelopeListWidget.cpp
    EnvelopeListWidget.hpp
    EnvelopeSplitModel.cpp
    EnvelopeSplitModel.hpp
    EnvelopeTransactionModel.cpp
    EnvelopeTransactionModel.hpp
    JournalEntryDialog.cpp
    JournalEntryDialog.hpp
    TransactionTableView.cpp
    TransactionTableView.hpp
    TreeView.cpp
    TreeView.hpp
)
qt5_use_modules(ledger_ui Core Script Widgets)
target_link_libraries(ledger_ui ledger_persistence ledger_model)
set_target_properties(ledger_ui PROPERTIES AUTOMOC ON)
