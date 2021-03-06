# UnderBudget test CMake configuration

if (NOT SKIP_TESTS)

	# Macro to build a unit test and add it to CTest
	macro(build_test name libs)
		add_executable(${name} ${name}.cpp)
		qt5_use_modules(${name} Core Widgets Test Sql)
		target_link_libraries(${name} ${libs})
		add_test(${name} ${CMAKE_CURRENT_BINARY_DIR}/${name})
	endmacro(build_test)

	# Add test sub-directories
	add_subdirectory(accounting)
	add_subdirectory(analysis)
	add_subdirectory(budget)
	add_subdirectory(ledger)

	# Add a target to create lcov coverage report (only debug/gcc)
	if (CMAKE_BUILD_TYPE MATCHES Debug)
		if (CMAKE_COMPILER_IS_GNUCXX)

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

		endif(CMAKE_COMPILER_IS_GNUCXX)
	endif(CMAKE_BUILD_TYPE MATCHES Debug)

endif(NOT SKIP_TESTS)
