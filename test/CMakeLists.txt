# UnderBudget test CMake configuration

# Macro to build a unit test and add it to CTest
macro(build_test name libs)
	add_executable(${name} ${name}.cpp)
	qt5_use_modules(${name} Core Widgets Test Sql)
	target_link_libraries(${name} ${libs})
	add_test(${name} ${CMAKE_CURRENT_BINARY_DIR}/${name})
endmacro(build_test)

# Add test sub-directories
add_subdirectory(accounting)
add_subdirectory(budget)
add_subdirectory(ledger)

