from PyQt5.QtCore import pyqtSignal
from PyQt5.QtCore import Qt
from PyQt5.QtCore import QByteArray
from PyQt5.QtCore import QSettings
from PyQt5.QtCore import QSize
from PyQt5.QtGui import QKeySequence
from PyQt5.QtWidgets import QAction
from PyQt5.QtWidgets import qApp
from PyQt5.QtWidgets import QMainWindow
from PyQt5.QtWidgets import QMessageBox
from PyQt5.QtWidgets import QProgressBar


class MainWindow(QMainWindow):
    """Main application window"""

    # Settings keys
    MAIN_WINDOW_SIZE = 'MainWindowSize'
    MAIN_WINDOW_STATE = 'MainWindowState'

    open_database = pyqtSignal()
    import_data = pyqtSignal()
    exit_application = pyqtSignal()
    add_transaction = pyqtSignal()
    view_accounts = pyqtSignal()
    view_envelopes = pyqtSignal()
    view_budgeted_incomes = pyqtSignal()
    view_budgeted_expenses = pyqtSignal()
    view_reports = pyqtSignal()
    about_application = pyqtSignal()
    about_qt = pyqtSignal()

    def __init__(self):
        super().__init__()
        self.restoreSettings()
        self.setWindowTitle(qApp.applicationName())
        self.setUnifiedTitleAndToolBarOnMac(True)

        self._progress_bar = QProgressBar()
        self._progress_bar.setTextVisible(False)
        self._progress_bar.setMinimum(0)
        self._progress_bar.setMaximum(100)
        self._progress_bar.setVisible(False)
        self.statusBar().addPermanentWidget(self._progress_bar)

        self._setupMenuBar()

        self.statusBar().showMessage('Ready')

    def closeEvent(self, event):
        """Save window state before closing the window"""
        self.saveSettings()
        super().closeEvent(event)

    def restoreSettings(self):
        """Restore saved window settings."""
        settings = QSettings()
        self.restoreState(settings.value(self.MAIN_WINDOW_STATE, QByteArray(), type=QByteArray))
        self.resize(settings.value(self.MAIN_WINDOW_SIZE, QSize(800, 700), type=QSize))

    def saveSettings(self):
        """Store current window settings, such as size, geometry, etc."""
        settings = QSettings()
        settings.setValue(self.MAIN_WINDOW_SIZE, self.size())
        settings.setValue(self.MAIN_WINDOW_STATE, self.saveState())

    def _about(self):
        title = self.tr('About {0}').format(qApp.applicationName())
        about = '<html><b><p>{name}</p></b>' \
            + '<p>{description}</p>' \
            + '{version_lbl}: {version}<br>' \
            + '{authors_lbl}: {authors}<br>' \
            + '{url_lbl}: <a href="http://{url}">{url}</a><br>' \
            + '{license_lbl}: GNU General Public License, version 3<br>' \
            + '{copyright_lbl}: 2018, {authors}<br><br>' \
            + '{icons_lbl}: IcoMoon {icons_descrip} ' \
            + '(<a href="https://creativecommons.org/licenses/by/4.0">CC by 4.0</a>)'
        about = about.format(name=qApp.applicationName(),
                             description=self.tr('Personal finance and budget manager.'),
                             version_lbl=self.tr('Version'), version=qApp.applicationVersion(),
                             authors_lbl=self.tr('Authors'), authors='Kyle Treubig',
                             url_lbl=self.tr('Homepage'), url=qApp.organizationDomain(),
                             license_lbl=self.tr('License'), copyright_lbl=self.tr('Copyright'),
                             icons_lbl=self.tr('Icons'), icons_descrip=self.tr('free icon pack'))
        msg = QMessageBox()
        msg.about(self, title, about)

    def _setupMenuBar(self):
        """Creates menu bar actions"""
        open_action = QAction(self.tr('&Open...'), self)
        open_action.setShortcut(QKeySequence.Open)
        open_action.setStatusTip(self.tr('Open database'))

        import_data = QAction(self.tr('&Import...'), self)
        import_data.setStatusTip(self.tr('Import data'))

        exit_action = QAction(self.tr('E&xit'), self)
        exit_action.setShortcut(QKeySequence.Quit)
        exit_action.setStatusTip(self.tr('Quit the application'))
        exit_action.triggered.connect(qApp.closeAllWindows)

        add_trn = QAction(self.tr('&Add Transaction...'), self)
        add_trn.setShortcut(QKeySequence.New)
        add_trn.setStatusTip(self.tr('Create a new transaction'))

        accounts = QAction(self.tr('&Accounts'), self)
        accounts.setShortcut(QKeySequence(Qt.Key_F5))
        accounts.setStatusTip(self.tr('View accounts'))

        envelopes = QAction(self.tr('&Envelopes'), self)
        envelopes.setShortcut(QKeySequence(Qt.Key_F6))
        envelopes.setStatusTip(self.tr('View envelopes'))

        incomes = QAction(self.tr('Budgeted &Incomes'), self)
        incomes.setShortcut(QKeySequence(Qt.Key_F7))
        incomes.setStatusTip(self.tr('View budgeted incomes'))

        expenses = QAction(self.tr('Budgeted E&xpenses'), self)
        expenses.setShortcut(QKeySequence(Qt.Key_F8))
        expenses.setStatusTip(self.tr('View budgeted expenses'))

        reports = QAction(self.tr('&Reports'), self)
        reports.setShortcut(QKeySequence(Qt.Key_F9))
        reports.setStatusTip(self.tr('View reports'))

        about = QAction(self.tr('&About {0}').format(qApp.applicationName()), self)
        about.setStatusTip(self.tr('About the application'))
        about.triggered.connect(self._about)

        about_qt = QAction(self.tr('About &Qt'), self)
        about_qt.setStatusTip(self.tr('About the Qt library'))
        about_qt.triggered.connect(qApp.aboutQt)

        file = self.menuBar().addMenu(self.tr('&File'))
        file.addAction(open_action)
        file.addAction(import_data)
        file.addSeparator()
        file.addAction(exit_action)

        edit = self.menuBar().addMenu(self.tr('&Edit'))
        edit.addAction(add_trn)

        view = self.menuBar().addMenu(self.tr('&View'))
        view.addAction(accounts)
        view.addAction(envelopes)
        view.addAction(incomes)
        view.addAction(expenses)
        view.addAction(reports)

        self.menuBar().addSeparator()

        help = self.menuBar().addMenu(self.tr('&Help'))
        help.addAction(about)
        help.addAction(about_qt)
