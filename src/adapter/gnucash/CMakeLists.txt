# GnuCash adapter source CMake configuration

# Specify GnuCash adapter source files
set(adapter_gnucash_srcs
    GnuCashImporter.cpp
)

# Build GnuCash adapter library
add_library(adapter_gnucash ${adapter_gnucash_srcs})
qt5_use_modules(adapter_gnucash Core Sql)
target_link_libraries(adapter_gnucash ledger_model)
