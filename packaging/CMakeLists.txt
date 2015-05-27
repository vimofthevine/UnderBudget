# Packaging CMake configuration

if (UNIX)
	# Add desktop file to install target
	install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/underbudget.desktop
		DESTINATION share/applications)

	# Add icon file to install target
	install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../logo.svg
		DESTINATION share/pixmaps
		RENAME underbudget.svg)

	set(CPACK_GENERATOR TGZ)

	# Create RPM only when explicitly requested
	if (CREATE_RPM)
		list(APPEND CPACK_GENERATOR RPM)
	endif (CREATE_RPM)

	# Create DEB only when explicitly requested
	if (CREATE_DEB)
		list(APPEND CPACK_GENERATOR DEB)
	endif (CREATE_DEB)

	# Create ArchLinux source tarball for the AUR
	add_custom_target(pkgbuild
		makepkg --source -p ${CMAKE_CURRENT_SOURCE_DIR}/PKGBUILD-git
		WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
		COMMENT "Generate ArchLinux PKGBUILD source tarball(s)" VERBATIM
	)
endif (UNIX)

# Setup CPack
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Advanced personal budget analysis")
set(CPACK_PACKAGE_VENDOR "vimofthevine")
set(CPACK_PACKAGE_VERSION ${APP_VERSION})
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Kyle Treubig")
include(CPack)
