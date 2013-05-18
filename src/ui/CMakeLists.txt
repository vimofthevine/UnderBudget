# UI source CMake configuration

# Specify UI source files
set(ui_srcs
	MainWindow.cpp
	MainWindow_actions.cpp
	MainWindow_menus.cpp
	Session.cpp
)

# Specify UI resource file
set(ui_rcs ui/ui.qrc PARENT_SCOPE)

# Build UI library
add_library(ui ${ui_srcs})
qt5_use_modules(ui Core Widgets)

