// Qt include(s)
#include <QtWidgets>

// UnderBudget include(s)
#include "MainWindow.hpp"
#include "MenuBar.hpp"
#include "ToolBar.hpp"

namespace ub {

//--------------------------------------------------------------------------------------------------
const QString MAIN_WINDOW_SIZE = "MainWindowSize";
const QString MAIN_WINDOW_STATE = "MainWindowState";

//--------------------------------------------------------------------------------------------------
MainWindow::MainWindow()
        : menu_(new MenuBar(this)), toolbar_(new ToolBar(this)), content_(new QStackedWidget(this)),
          progress_bar_(new QProgressBar) {
    restoreSettings();

    setWindowTitle(qApp->applicationName());
    setWindowIcon(QIcon(":/logo"));
    setUnifiedTitleAndToolBarOnMac(true);
    setAttribute(Qt::WA_DeleteOnClose, true);

    progress_bar_->setMaximumSize(200, 50);
    progress_bar_->setTextVisible(false);
    progress_bar_->setMinimum(0);
    progress_bar_->setMaximum(100);
    progress_bar_->setVisible(false);
    statusBar()->addPermanentWidget(progress_bar_);

    connect(menu_, &MenuBar::exitApplication, qApp, &QApplication::closeAllWindows);
    connect(menu_, &MenuBar::aboutApplication, this, &MainWindow::about);
    connect(menu_, &MenuBar::aboutQt, qApp, QApplication::aboutQt);
    setMenuBar(menu_);
    addToolBar(toolbar_);

    setCentralWidget(content_);
}

//--------------------------------------------------------------------------------------------------
MenuBar * MainWindow::menu() const {
    return menu_;
}

//--------------------------------------------------------------------------------------------------
ToolBar * MainWindow::toolBar() const {
    return toolbar_;
}

//--------------------------------------------------------------------------------------------------
QStackedWidget * MainWindow::contentWidget() const {
    return content_;
}

//--------------------------------------------------------------------------------------------------
void MainWindow::showStatusMessage(const QString & message) {
    statusBar()->showMessage(message, 2000);
}

//--------------------------------------------------------------------------------------------------
void MainWindow::showProgress(int value, int max) {
    // If indefinite
    if (max == 0) {
        progress_bar_->setVisible(true);
    } else {
        // If finished, hide the progress bar
        progress_bar_->setVisible(value != max);
    }

    progress_bar_->setMinimum(0);
    progress_bar_->setMaximum(max);
    progress_bar_->setValue(value);
}

//--------------------------------------------------------------------------------------------------
void MainWindow::closeEvent(QCloseEvent * event) {
    saveSettings();
    QMainWindow::closeEvent(event);
}

//--------------------------------------------------------------------------------------------------
void MainWindow::saveSettings() {
    QSettings settings;
    settings.setValue(MAIN_WINDOW_SIZE, size());
    settings.setValue(MAIN_WINDOW_STATE, saveState());
}

//--------------------------------------------------------------------------------------------------
void MainWindow::restoreSettings() {
    QSettings settings;
    restoreState(settings.value(MAIN_WINDOW_STATE, QByteArray()).toByteArray());
    resize(settings.value(MAIN_WINDOW_SIZE, QSize(800, 700)).toSize());
}

//--------------------------------------------------------------------------------------------------
void MainWindow::about() {
    QString title = tr("About %1").arg(qApp->applicationName());
    QString about =
        QString("<html><b><p>%1</p></b>") + "<p>" + tr("Personal finance and budget manager.") +
        "</p>" + tr("Version") + ": %2<br>" + tr("Authors") + ": Kyle Treubig<br>" +
        tr("Homepage") + ": <a href=\"http://%3\">%3</a><br>" + tr("License") +
        ": GNU General Public License, version 3<br>" + tr("Copyright") +
        " &copy; 2017, Kyle Treubig<br><br>" + tr("Icons") + ": IcoMoon " + tr("free icon pack") +
        " (<a href=\"https://creativecommons.org/licenses/by/4.0\">CC by 4.0</a>)";
    about = about.arg(qApp->applicationName())
                .arg(qApp->applicationVersion())
                .arg(qApp->organizationDomain());

    QMessageBox::about(this, title, about);
}

} // namespace ub
