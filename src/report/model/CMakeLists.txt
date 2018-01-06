# Build report model library
define_library(report_model
    Compiler.cpp
    Compiler.hpp
)
qt5_use_modules(report_model Core Charts)
target_link_libraries(report_model budget_model ledger_model)
