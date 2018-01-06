# Find the GoogleTest libraries
find_package(GTest REQUIRED)
_gtest_find_library(GMOCK_LIBRARY gmock-md gmock)
_gtest_find_library(GMOCK_LIBRARY_DEBUG gmock-mdd gmockd)
_gtest_append_debugs(GMOCK_LIBRARIES GMOCK_LIBRARY)

# Find the Qt libraries
find_package(Qt5Widgets REQUIRED)

# Function to build a QtTest unit test and add it to CTest
function(build_test name libs)
    add_executable(${name} ${name}.cpp)
    target_include_directories(${name} PRIVATE ${PROJECT_SOURCE_DIR}/src)
    qt5_use_modules(${name} Core Widgets Test Sql)
    target_link_libraries(${name} ${libs})
    set_target_properties(${name} PROPERTIES AUTOMOC ON)
    add_test(${name} ${CMAKE_CURRENT_BINARY_DIR}/${name})
endfunction(build_test)

# Function to build a GoogleTest unit test and add it to CTest
function(build_gtest name libs)
    add_executable(${name} ${name}.cpp)
    target_include_directories(${name} PRIVATE ${GTEST_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR}/src)
    qt5_use_modules(${name} Core Sql Widgets)
    target_link_libraries(${name} ${libs} ${GTEST_BOTH_LIBRARIES} ${GMOCK_LIBRARIES} pthread)
    add_test(${name} ${CMAKE_CURRENT_BINARY_DIR}/${name})
    # Not sure whether tests should run automatically after being built
    #add_custom_command(TARGET ${name} POST_BUILD COMMAND ${name})
endfunction(build_gtest)

# Add test sub-directories
add_subdirectory(budget)
add_subdirectory(ledger)

# Add a target to create lcov coverage report (only debug/gcc)
if (CMAKE_BUILD_TYPE MATCHES Debug)
    if (CMAKE_COMPILER_IS_GNUCXX)
        if (ENABLE_GCOV)
            # Make sure lcov and genhtml are available
            find_program(LCOV_PATH lcov)
            find_program(GENHTML_PATH genhtml)

            if (NOT LCOV_PATH)
                message(WARNING "lcov not found! Unable to generate coverage reports.")
            endif (NOT LCOV_PATH)

            if (NOT GENHTML_PATH)
                message(WARNING "genhtml not found! Unable to generate coverage reports.")
            endif (NOT GENHTML_PATH)

            # Create coverage reset target
            add_custom_target(coverage_reset
                # Cleanup lcov
                ${LCOV_PATH} --directory . --zerocounters
                WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                COMMENT "Resetting code coverage counters to zero."
                )

            # Create coverage report target
            add_custom_target(coverage_report
                COMMAND ${LCOV_PATH} --directory . --capture --output-file coverage.info
                COMMAND ${LCOV_PATH} --remove coverage.info 'moc_*' 'test/*' '/usr/*' --output-file coverage.info.cleaned
                COMMAND ${GENHTML_PATH} -o coverage coverage.info.cleaned
                COMMAND ${CMAKE_COMMAND} -E remove coverage.info coverage.info.cleaned
                WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                COMMENT "Processing code coverage counters and generating report."
                )
        endif (ENABLE_GCOV)
    endif (CMAKE_COMPILER_IS_GNUCXX)
endif (CMAKE_BUILD_TYPE MATCHES Debug)
