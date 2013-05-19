# UI wizard source CMake configuration

# Specify UI wizard source files
set(ui_wizard_srcs
	BudgetFileWizard.cpp
)

# Build UI wizard library
add_library(ui_wizard ${ui_wizard_srcs})
qt5_use_modules(ui_wizard Core Widgets)

