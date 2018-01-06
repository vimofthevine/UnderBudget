# Build adapter wizard library
define_library(adapter_wizard
    GnuCashSourcePage.cpp
    GnuCashSourcePage.hpp
    ImportDataWizard.cpp
    ImportDataWizard.hpp
    SourceSelectionPage.cpp
    SourceSelectionPage.hpp
    WizardPageId.hpp
)
qt5_use_modules(adapter_wizard Core Widgets)
target_link_libraries(adapter_wizard adapter_gnucash)
set_target_properties(adapter_wizard PROPERTIES AUTOMOC ON)
