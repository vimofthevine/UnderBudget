// Qt include(s)
#include <QtWidgets>

// UnderBudget include(s)
#include "MainWindow.hpp"
#include "MainWindowModel.hpp"
#include "MenuBar.hpp"

namespace ub {

//--------------------------------------------------------------------------------------------------
const QString MAIN_WINDOW_SIZE = "MainWindowSize";
const QString MAIN_WINDOW_STATE = "MainWindowState";

//--------------------------------------------------------------------------------------------------
MainWindow::MainWindow() : content_(new QStackedWidget(this)) {
    restoreSettings();

    setWindowTitle(qApp->applicationName());
    setWindowIcon(QIcon(":/logo"));
    setUnifiedTitleAndToolBarOnMac(true);
    setAttribute(Qt::WA_DeleteOnClose, true);

    auto menu = new MenuBar(this);
    connect(menu, &MenuBar::openDatabase, this, &MainWindow::openDatabase);
    connect(menu, &MenuBar::exitApplication, qApp, &QApplication::closeAllWindows);
    connect(menu, &MenuBar::aboutApplication, this, &MainWindow::about);
    connect(menu, &MenuBar::aboutQt, qApp, QApplication::aboutQt);
    setMenuBar(menu);

    setCentralWidget(content_);
}

//--------------------------------------------------------------------------------------------------
QStackedWidget *MainWindow::contentWidget() {
    return content_;
}

//--------------------------------------------------------------------------------------------------
void MainWindow::closeEvent(QCloseEvent *event) {
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
    QString about = QString("<html><b><p>%1</p></b>")
        + "<p>"
        + tr("Personal finance and budget manager.")
        + "</p>"
        + tr("Version") + ": %2<br>"
        + tr("Authors") + ": Kyle Treubig<br>"
        + tr("Homepage") + ": <a href=\"http://%3\">%3</a><br>"
        + tr("License") + ": Apache License 2.0<br>"
        + tr("Copyright") + " &copy; 2013, Kyle Treubig";
    about = about.arg(qApp->applicationName())
        .arg(qApp->applicationVersion())
        .arg(qApp->organizationDomain());

    QMessageBox::about(this, title, about);
}

} // ub namespace