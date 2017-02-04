# Application UI source CMake configuration

# Specify application UI source files
set(app_ui_srcs
    DatabaseFileChooser.cpp
    MainWindow.cpp
)

# Build ledger UI library
add_library(app_ui ${app_ui_srcs})
qt5_use_modules(app_ui Core Widgets)
target_link_libraries(app_ui app_model)
