# Report UI source CMake configuration

# Specify report UI source files
set(report_ui_srcs
    ImpactModel.cpp
    ProjectedExpenseModel.cpp
    ProjectedIncomeModel.cpp
    ReportWidget.cpp
)

# Build report UI library
add_library(report_ui ${report_ui_srcs})
qt5_use_modules(report_ui Charts Core Widgets)
target_link_libraries(report_ui report_model)
