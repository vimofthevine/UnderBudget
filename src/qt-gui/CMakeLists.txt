# Qt-GUI module

# Configure the application info header
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/info.hpp.in
	${CMAKE_CURRENT_BINARY_DIR}/info.hpp)

# Specify module source files
set(module_srcs
	Activator.cpp
	QtApplication.cpp
)

# Generate module initialization code
usFunctionGenerateModuleInit(module_srcs)

# Build module library
add_library(qtgui SHARED ${module_srcs})
qt5_use_modules(qtgui Core Widgets)
target_link_libraries(qtgui ui)
target_link_libraries(qtgui ${CppMicroServices_LIBRARIES})
set_property(TARGET qtgui APPEND PROPERTY COMPILE_DEFINITIONS
	US_MODULE_NAME=qtgui)
