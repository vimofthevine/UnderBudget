# UnderBudget installer
# Maintainer: Kyle Treubig <kyle [at] vimofthevine [dot] com>

ANT = ant
INSTALL = install

prefix = $(DESTDIR)/usr
BINDIR = $(prefix)/bin
DATADIR = $(prefix)/share
DESKTOPDIR = $(DATADIR)/applications
ICONDIR = $(DATADIR)/pixmaps
JARDIR = $(DATADIR)/java/underbudget
APPDATA = $(DATADIR)/underbudget
gtk_update_icon_cache = gtk-update-icon-cache -f -t $(ICONPATH)

CLIDESKTOPFILE = underbudgetcli.desktop
ICONFILE = underbudget.png
CLIJAR = UnderBudgetCli.jar
LOGFILE = logging.properties
CLIBIN = underbudgetcli

install: $(CLIJAR)
	$(INSTALL) -d -m 755 $(DESKTOPDIR)
	$(INSTALL) -m 644 data/$(CLIDESKTOPFILE) $(DESKTOPDIR)
	$(INSTALL) -d -m 755 $(ICONDIR)
	$(INSTALL) -m 644 data/$(ICONFILE) $(ICONDIR)
	$(INSTALL) -d -m 755 $(JARDIR)
	$(INSTALL) -m 644 build/jar/$(CLIJAR) $(JARDIR)
	$(INSTALL) -d -m 755 $(APPDATA)
	$(INSTALL) -m 644 data/$(LOGFILE) $(APPDATA)
	$(INSTALL) -d -m 755 $(BINDIR)
	$(INSTALL) -m 755 scripts/$(CLIBIN) $(BINDIR)

uninstall:
	rm $(DESKTOPDIR)/$(CLIDESKTOPFILE)
	rm $(ICONDIR)/$(ICONFILE)
	rm -r $(JARDIR)
	rm -r $(APPDATA)/$(LOGFILE)
	rm $(BINDIR)/$(CLIBIN)

$(CLIJAR):
	$(ANT) cli

