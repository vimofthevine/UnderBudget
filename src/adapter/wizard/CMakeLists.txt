# Adapter wizard source CMake configuration

# Specify adapter wizard source files
set(adapter_wizard_srcs
    GnuCashSourcePage.cpp
    ImportDataWizard.cpp
    SourceSelectionPage.cpp
)

# Build adapter wizard library
add_library(adapter_wizard ${adapter_wizard_srcs})
qt5_use_modules(adapter_wizard Core Widgets)
target_link_libraries(adapter_wizard adapter_gnucash)
