// Qt include(s)
#include <QtWidgets>

// UnderBudget include(s)
#include "MainWindow.hpp"

namespace ub {

//--------------------------------------------------------------------------------------------------
const QString MAIN_WINDOW_SIZE = "MainWindowSize";
const QString MAIN_WINDOW_STATE = "MainWindowState";

//--------------------------------------------------------------------------------------------------
MainWindow::MainWindow() {
    restoreSettings();

    setWindowTitle(qApp->applicationName());
    setWindowIcon(QIcon(":/logo"));
    setUnifiedTitleAndToolBarOnMac(true);
    setAttribute(Qt::WA_DeleteOnClose, true);
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

} // ub namespace
