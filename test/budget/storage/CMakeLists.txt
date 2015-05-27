# Budget test CMake configuration

# Build unit tests
build_test(XmlBudgetReaderTest budget_storage)
build_test(XmlBudgetReaderV4Test budget_storage)
build_test(XmlBudgetReaderV5Test budget_storage)

# Have to setup this test separately as it has extra
# dependencies and resources
qt5_add_resources(schema_resources schema.qrc)
add_executable(XmlBudgetWriterTest XmlBudgetWriterTest.cpp ${schema_resources})
qt5_use_modules(XmlBudgetWriterTest Core Test XmlPatterns)
target_link_libraries(XmlBudgetWriterTest budget_storage)
add_test(XmlBudgetWriterTest ${CMAKE_CURRENT_BINARY_DIR}/XmlBudgetWriterTest)

