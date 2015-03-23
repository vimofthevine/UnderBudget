# Accounting source CMake configuration

# Specify accounting source files
set(accounting_srcs
	currencies.cpp
	ConversionRatesSource.cpp
	Currency.cpp
	Money.cpp
	UserConversionRates.cpp
)

# Build accounting library
add_library(accounting ${accounting_srcs})
qt5_use_modules(accounting Core Sql)

