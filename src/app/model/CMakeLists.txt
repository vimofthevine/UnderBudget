# Build application model library
define_library(app_model
    DatabaseRepositories.cpp
    DatabaseRepositories.hpp
    Demo.hpp
    Demo.cpp
    Repositories.hpp
)
qt5_use_modules(app_model Core)
target_link_libraries(app_model budget_persistence ledger_persistence)
