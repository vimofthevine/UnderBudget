# Find the Qt libraries
find_package(Qt5Widgets REQUIRED)

# Function to configure static library modules with global settings
function(define_library name)
    add_library(${name} STATIC ${ARGN})
    # So all includes will be relative to the src folder
    target_include_directories(${name} PRIVATE ${PROJECT_SOURCE_DIR}/src)
    if (CMAKE_BUILD_TYPE MATCHES Debug)
        # Enable extra debugging options if debug build type
        target_compile_definitions(${name} PRIVATE QT_SHAREDPOINTER_TRACK_POINTERS)
        if (CMAKE_COMPILER_IS_GNUCXX)
            target_compile_options(${name} PRIVATE -O0)
            if (ENABLE_GCOV)
                target_compile_options(${name} PUBLIC -fprofile-arcs -ftest-coverage)
                target_link_libraries(${name} gcov)
            endif (ENABLE_GCOV)
        endif (CMAKE_COMPILER_IS_GNUCXX)
    endif (CMAKE_BUILD_TYPE MATCHES Debug)
    if (CMAKE_BUILD_TYPE MATCHES Release)
        target_compile_definitions(${name} PRIVATE QT_NO_DEBUG_OUTPUT)
    endif (CMAKE_BUILD_TYPE MATCHES Release)
endfunction(define_library name)

# Add sub-directories
add_subdirectory(adapter)
add_subdirectory(app)
add_subdirectory(app/resources)
add_subdirectory(budget)
add_subdirectory(ledger)
add_subdirectory(report)

# Instruct CMake to place executable in the bin folder
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

# Set app release type property to dev when not a release build
if (NOT CMAKE_BUILD_TYPE MATCHES Release)
    set(APP_REL_TYPE "-dev")
endif (NOT CMAKE_BUILD_TYPE MATCHES Release)

# Configure the application info header
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/info.hpp.in
    ${CMAKE_CURRENT_BINARY_DIR}/info.hpp)

# Specify resource files
# (this can't be done directly in a subdirectory,
# so sub-projects should set their respective _rcs variables)
qt5_add_resources(ui_rcs_srcs ${ui_rcs})

# If windows, make sure we don't generate a console application
# in a Release build
if (WIN32)
    if (CMAKE_BUILD_TYPE MATCHES Release)
        set(CMAKE_EXE_LINKER_FLAGS "-mwindows")
    endif (CMAKE_BUILD_TYPE MATCHES Release)
endif (WIN32)

# Build underbudget executable
add_executable(underbudget main.cpp ${ui_rcs_srcs})
target_link_libraries(underbudget app_model app_ui)
qt5_use_modules(underbudget Core Sql Widgets)

# Add executable to install target
install(PROGRAMS ${PROJECT_BINARY_DIR}/bin/underbudget DESTINATION bin)
