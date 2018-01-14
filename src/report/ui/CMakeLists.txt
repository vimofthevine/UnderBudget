# Build report UI library
define_library(report_ui
    ImpactModel.cpp
    ImpactModel.hpp
    ProjectedExpenseModel.cpp
    ProjectedExpenseModel.hpp
    ProjectedIncomeModel.cpp
    ProjectedIncomeModel.hpp
    ReportWidget.cpp
    ReportWidget.hpp
)
qt5_use_modules(report_ui Charts Core Widgets)
target_link_libraries(report_ui report_model)
set_target_properties(report_ui PROPERTIES AUTOMOC ON)
