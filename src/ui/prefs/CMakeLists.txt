# UI preferences source CMake configuration

# Specify UI preferences source files
set(ui_prefs_srcs
	AppearanceSettings.cpp
	GeneralSettings.cpp
	PrefsDialog.cpp
)

# Build UI preferences library
add_library(ui_prefs ${ui_prefs_srcs})
qt5_use_modules(ui_prefs Core Widgets)

