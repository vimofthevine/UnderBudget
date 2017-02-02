# Ledger UI test CMake configuration

# Build test executables
add_executable(AccountListWidgetTest AccountListWidgetTest.cpp)
target_link_libraries(AccountListWidgetTest ledger_ui)
qt5_use_modules(AccountListWidgetTest Core Widgets)
