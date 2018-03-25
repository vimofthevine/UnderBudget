# UnderBudget
# Copyright 2018 Kyle Treubig
#
# UnderBudget is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# UnderBudget is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with UnderBudget.  If not, see <http://www.gnu.org/licenses/>.


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
from PyQt5.QtWidgets import QStackedWidget
from PyQt5.QtWidgets import QToolBar


class MainWindow(QMainWindow):
    """Main application window"""

    # Settings keys
    MAIN_WINDOW_SIZE = 'MainWindowSize'
    MAIN_WINDOW_STATE = 'MainWindowState'

    open_database = pyqtSignal()
    import_data = pyqtSignal()
    add_transaction = pyqtSignal()
    view_accounts = pyqtSignal()
    view_envelopes = pyqtSignal()
    view_budgeted_incomes = pyqtSignal()
    view_budgeted_expenses = pyqtSignal()
    view_reports = pyqtSignal()

    def __init__(self):
        """Initialize the main window, menu, toolbar, and status bar"""
        super().__init__()
        self.restore_settings()

        self.setWindowTitle(qApp.applicationName())
        self.setUnifiedTitleAndToolBarOnMac(True)

        self._progress_bar = QProgressBar()
        self._progress_bar.setTextVisible(False)
        self._progress_bar.setMinimum(0)
        self._progress_bar.setMaximum(100)
        self._progress_bar.setVisible(False)
        self.statusBar().addPermanentWidget(self._progress_bar)

        self._setup_menu_bar()
        self._setup_tool_bar()

        self.content = QStackedWidget(self)

        self.statusBar().showMessage('Ready')

    def closeEvent(self, event):
        """Save window state before closing the window"""
        self.save_settings()
        super().closeEvent(event)

    def restore_settings(self):
        """Restore saved window settings."""
        settings = QSettings()
        self.restoreState(settings.value(self.MAIN_WINDOW_STATE, QByteArray(), type=QByteArray))
        self.resize(settings.value(self.MAIN_WINDOW_SIZE, QSize(800, 700), type=QSize))

    def save_settings(self):
        """Store current window settings, such as size, geometry, etc."""
        settings = QSettings()
        settings.setValue(self.MAIN_WINDOW_SIZE, self.size())
        settings.setValue(self.MAIN_WINDOW_STATE, self.saveState())

    def show_progress(self, value, max_):
        """Display the specified progress percentage in the window status bar.

        If value and max are 0, then an indefinite, or busy, progress will be displayed.

        If both value and max are the same, the progress bar will be hidden.
        """
        if max_ == 0:
            self._progress_bar.setVisible(True)
        else:
            self._progress_bar.setVisible(value != max_)
        self._progress_bar.setMinimum(0)
        self._progress_bar.setMaximum(max_)
        self._progress_bar.setValue(value)

    def show_status_message(self, message):
        """Display the given message temporarily in the window status bar."""
        self.statusBar().showMessage(message, 2000)

    def _show_about(self):
        """Show about dialog with information about the application."""
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

    def _setup_menu_bar(self):
        """Creates menu bar actions"""
        open_ = QAction(self.tr('&Open...'), self)
        open_.setShortcut(QKeySequence.Open)
        open_.setStatusTip(self.tr('Open database'))

        import_ = QAction(self.tr('&Import...'), self)
        import_.setStatusTip(self.tr('Import data'))

        exit_ = QAction(self.tr('E&xit'), self)
        exit_.setShortcut(QKeySequence.Quit)
        exit_.setStatusTip(self.tr('Quit the application'))
        exit_.triggered.connect(qApp.closeAllWindows)

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
        about.triggered.connect(self._show_about)

        about_qt = QAction(self.tr('About &Qt'), self)
        about_qt.setStatusTip(self.tr('About the Qt library'))
        about_qt.triggered.connect(qApp.aboutQt)

        file = self.menuBar().addMenu(self.tr('&File'))
        file.addAction(open_)
        file.addAction(import_)
        file.addSeparator()
        file.addAction(exit_)

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

    def _setup_tool_bar(self):
        """Creates tool bar actions"""
        add_trn = QAction(self.tr('&Add Transaction...'), self)
        add_trn.setStatusTip(self.tr('Create a new transaction'))

        accounts = QAction(self.tr('&Accounts'), self)
        accounts.setStatusTip(self.tr('View accounts'))

        envelopes = QAction(self.tr('&Envelopes'), self)
        envelopes.setStatusTip(self.tr('View envelopes'))

        incomes = QAction(self.tr('Budgeted &Incomes'), self)
        incomes.setStatusTip(self.tr('View budgeted incomes'))

        expenses = QAction(self.tr('Budgeted E&xpenses'), self)
        expenses.setStatusTip(self.tr('View budgeted expenses'))

        reports = QAction(self.tr('&Reports'), self)
        reports.setShortcut(QKeySequence(Qt.Key_F9))
        reports.setStatusTip(self.tr('View reports'))

        tool_bar = QToolBar(self)
        tool_bar.setToolButtonStyle(Qt.ToolButtonTextUnderIcon)
        tool_bar.setFloatable(False)
        tool_bar.setMovable(False)

        tool_bar.addAction(add_trn)
        tool_bar.addSeparator()
        tool_bar.addAction(accounts)
        tool_bar.addAction(envelopes)
        tool_bar.addAction(incomes)
        tool_bar.addAction(expenses)
        tool_bar.addAction(reports)
        self.addToolBar(tool_bar)
