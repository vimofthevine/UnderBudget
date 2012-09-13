#
# Generic Unix install/uninstall script
#

install:
	install -Dm644 data/underbudget.desktop ${pkgdir}/usr/share/applications/underbudget.desktop
	install -Dm644 data/underbudget.png ${pkgdir}/usr/share/pixmaps/underbudget.png
	install -Dm644 underbudget.jar ${pkgdir}/usr/share/java/underbudget/underbudget.jar
	install -Dm644 data/logging.properties ${pkgdir}/usr/share/underbudget/logging.properties
	install -Dm755 scripts/underbudget ${pkgdir}/usr/bin/underbudget

uninstall:
	rm /usr/share/applications/underbudget.desktop
	rm /usr/share/pixmaps/underbudget.png
	rm -r /usr/share/java/underbudget/
	rm -r /usr/share/underbudget/
	rm /usr/bin/underbudget

