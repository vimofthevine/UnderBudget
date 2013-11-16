# GZip support source CMake configuration

# Include zlib library
find_package(ZLIB REQUIRED)

# Specify GZip support source files
set(gzip_support_srcs
	GZipFile.cpp
)

# Build GZip support library
add_library(gzip_support ${gzip_support_srcs})
qt5_use_modules(gzip_support Core)
target_link_libraries(gzip_support ${QLIB_LIBRARIES})

