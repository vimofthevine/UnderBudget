# Application model source CMake configuration

# Specify application model source files
set(app_model_srcs
    DatabaseRepositories.cpp
    Demo.cpp
)

# Build application model library
add_library(app_model ${app_model_srcs})
qt5_use_modules(app_model Core)
target_link_libraries(app_model ledger_persistence)
