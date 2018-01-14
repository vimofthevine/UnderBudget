# Build ledger UI library
set(srcs
    DatabaseFileChooser.cpp
    DatabaseFileChooser.hpp
    MainWindow.cpp
    MainWindow.hpp
    MainWindowModel.cpp
    MainWindowModel.hpp
    MenuBar.cpp
    MenuBar.hpp
    ToolBar.cpp
    ToolBar.hpp
)
# Generic look-and-feel icons
list(APPEND srcs icons_icomoon.cpp)
# Platform-specific look-and-feel icons
#if(${CMAKE_SYSTEM_NAME} MATCHES Linux)
    #list(APPEND app_ui_srcs icons_tango.cpp)
#endif(${CMAKE_SYSTEM_NAME} MATCHES Linux)
define_library(app_ui ${srcs})
qt5_use_modules(app_ui Core Widgets)
target_link_libraries(app_ui app_model budget_ui ledger_ui report_ui adapter_wizard)
set_target_properties(app_ui PROPERTIES AUTOMOC ON)
