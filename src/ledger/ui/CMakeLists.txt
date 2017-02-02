# Ledger UI source CMake configuration

# Specify ledger UI source files
set(ledger_ui_srcs
    AccountModel.cpp
    AccountListWidget.cpp
)

# Build ledger UI library
add_library(ledger_ui ${ledger_ui_srcs})
qt5_use_modules(ledger_ui Core Widgets)
target_link_libraries(ledger_ui ledger_persistence ledger_model)
