# UI accounting source CMake configuration

# Specify UI accounting source files
set(ui_accounting_srcs
	MoneyDelegate.cpp
	MoneyEdit.cpp
	MoneyValidator.cpp
)

# Build UI accounting library
add_library(ui_accounting ${ui_accounting_srcs})
qt5_use_modules(ui_accounting Core Widgets)
target_link_libraries(ui_accounting accounting ui_widgets)

