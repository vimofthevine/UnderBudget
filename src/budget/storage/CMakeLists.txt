# Budget storage source CMake configuration

# Specify budget storage source files
set(budget_storage_srcs
	SqlBudgetFile.cpp
	XmlBudgetFile.cpp
	XmlBudgetReader.cpp
	XmlBudgetWriter.cpp
)

# Build budget storage library
add_library(budget_storage ${budget_storage_srcs})
qt5_use_modules(budget_storage Core)
target_link_libraries(budget_storage budget)

