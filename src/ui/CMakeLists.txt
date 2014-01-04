# UI source CMake configuration

# Add UI source sub-directories
add_subdirectory(accounting)
add_subdirectory(analysis)
add_subdirectory(budget)
add_subdirectory(ledger)
add_subdirectory(prefs)
add_subdirectory(widgets)
add_subdirectory(wizard)

# Specify UI source files
set(ui_srcs
	MainWindow.cpp
	MainWindow_actions.cpp
	MainWindow_menus.cpp
	MdiWindow.cpp
	SdiWindow.cpp
	Session.cpp
)

# Specify UI dependent libraries
set(ui_libs
	ui_analysis
	ui_budget
	ui_ledger
	ui_prefs
	ui_wizard
	analysis
	budget
)

# Specify UI resource file
if(ICON_SET MATCHES V2Style)
	set(ui_rcs ui/ui.qrc PARENT_SCOPE)
else()
	set(ui_rcs ui/oxygen.qrc PARENT_SCOPE)
endif (ICON_SET MATCHES V2Style)

# Build UI library
add_library(ui ${ui_srcs})
qt5_use_modules(ui Core Widgets)
target_link_libraries(ui ${ui_libs})

