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

# Add source sub-directories
add_subdirectory(accounting)
add_subdirectory(budget)
#add_subdirectory(engine)
add_subdirectory(ledger)
add_subdirectory(ui)

# Instruct CMake to place executable in the bin folder
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

# Configure the application info header
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/info.hpp.in
	${CMAKE_CURRENT_BINARY_DIR}/info.hpp)

# Specify underbudget source files
set(underbudget_srcs
	main.cpp
)

# Specify resource files
# (this can't be done directly in a subdirectory,
# so sub-projects should set their respective _rcs variables)
qt5_add_resources(ui_rcs_srcs ${ui_rcs})

# Build underbudget executable
add_executable(underbudget ${underbudget_srcs} ${ui_rcs_srcs})
target_link_libraries(underbudget ui)
qt5_use_modules(underbudget Core Widgets)

# Add executable to install target
install(PROGRAMS ${PROJECT_BINARY_DIR}/bin/underbudget DESTINATION bin)

