# UI analysis source CMake configuration

# Specify UI analysis source files
set(ui_analysis_srcs
	AnalysisSummaryWidget.cpp
	EstimateDiffsModel.cpp
	ProjectedBalanceModel.cpp
)

# Build UI budget library
add_library(ui_analysis ${ui_analysis_srcs})
qt5_use_modules(ui_analysis Core Widgets)
target_link_libraries(ui_analysis analysis)

