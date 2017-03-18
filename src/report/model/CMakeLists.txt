# Report model source CMake configuration

# Specify report model source files
set(report_model_srcs
    Compiler.cpp
)

# Build report model library
add_library(report_model ${report_model_srcs})
qt5_use_modules(report_model Core Charts)
target_link_libraries(report_model budget_model ledger_model)
