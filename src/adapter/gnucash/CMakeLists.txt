# Build GnuCash adapter library
define_library(adapter_gnucash
    GnuCashImporter.cpp
    GnuCashImporter.hpp
)
qt5_use_modules(adapter_gnucash Core Sql)
target_link_libraries(adapter_gnucash ledger_model)
set_target_properties(adapter_gnucash PROPERTIES AUTOMOC ON)
