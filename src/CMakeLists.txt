# UnderBudget source CMake configuration

# Disable debug print statements for release build
if (CMAKE_BUILD_TYPE MATCHES Release)
	add_definitions(-DQT_NO_DEBUG_OUTPUT)
endif (CMAKE_BUILD_TYPE MATCHES Release)

# Enable extra debugging options if debug build type
if (CMAKE_BUILD_TYPE MATCHES Debug)
	add_definitions(-DQT_SHAREDPOINTER_TRACK_POINTERS)
endif (CMAKE_BUILD_TYPE MATCHES Debug)

# If windows, make sure we don't generate a console application
# in a Release build
if (WIN32)
	if (CMAKE_BUILD_TYPE MATCHES Release)
		set(CMAKE_EXE_LINKER_FLAGS "-mwindows")
	endif (CMAKE_BUILD_TYPE MATCHES Release)
endif (WIN32)

# Put libraries in the plugins folder
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/plugins)
# Put executables in the bin folder
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

# Add source sub-directories
add_subdirectory(accounting)
add_subdirectory(analysis)
add_subdirectory(budget)
add_subdirectory(gzip)
add_subdirectory(ledger)
add_subdirectory(qt-gui)
add_subdirectory(ui)

# Specify underbudget source files
set(underbudget_srcs
	main.cpp
	PluginLoader.cpp
)

# Generate module initialization code
usFunctionGenerateModuleInit(underbudget_srcs)

# Specify resource files
# (this can't be done directly in a subdirectory,
# so sub-projects should set their respective _rcs variables)
qt5_add_resources(ui_rcs_srcs ${ui_rcs})

# Build underbudget executable
add_executable(underbudget ${underbudget_srcs} ${ui_rcs_srcs})
target_link_libraries(underbudget ${CppMicroServices_LIBRARIES})
qt5_use_modules(underbudget Core)
set_property(TARGET underbudget APPEND PROPERTY COMPILE_DEFINITIONS
	US_MODULE_NAME=main)

# Add executable to install target
install(TARGETS underbudget DESTINATION bin)
