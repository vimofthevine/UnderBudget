# UI widgets source CMake configuration

# Specify UI widgets source files
set(ui_widgets_srcs
	IgnoreUndoRedo.cpp
)

# Build UI widgets library
add_library(ui_widgets ${ui_widgets_srcs})
qt5_use_modules(ui_widgets Core Widgets)

