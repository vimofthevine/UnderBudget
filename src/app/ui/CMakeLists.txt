# Application UI source CMake configuration

# Specify application UI source files
set(app_ui_srcs
    DatabaseFileChooser.cpp
    MainWindow.cpp
    MainWindowModel.cpp
    MenuBar.cpp
    ToolBar.cpp
)

list(APPEND app_ui_srcs icons_icomoon.cpp)

# Platform-specific look-and-feel resources
if(${CMAKE_SYSTEM_NAME} MATCHES Linux)
    #list(APPEND app_ui_srcs icons_tango.cpp)
endif(${CMAKE_SYSTEM_NAME} MATCHES Linux)

# Build ledger UI library
add_library(app_ui ${app_ui_srcs})
qt5_use_modules(app_ui Core Widgets)
target_link_libraries(app_ui app_model budget_ui ledger_ui report_ui)
