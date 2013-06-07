# UI source CMake configuration

# Add UI source sub-directories
add_subdirectory(accounting)
add_subdirectory(budget)
add_subdirectory(widgets)
add_subdirectory(wizard)

# Specify UI source files
set(ui_srcs
	MainWindow.cpp
	MainWindow_actions.cpp
	MainWindow_menus.cpp
	Session.cpp
)

# Specify UI dependent libraries
set(ui_libs
	ui_budget
	ui_wizard
	budget
)

# Specify UI resource file
set(ui_rcs ui/ui.qrc PARENT_SCOPE)

# Build UI library
add_library(ui ${ui_srcs})
qt5_use_modules(ui Core Widgets)
target_link_libraries(ui ${ui_libs})

