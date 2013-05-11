# UI source CMake configuration

# Specify UI source files
set(ui_srcs
	MainWindow.cpp
)

# Build UI library
add_library(ui ${ui_srcs})
qt5_use_modules(ui Core Widgets)

