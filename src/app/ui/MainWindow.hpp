#pragma once

// Qt include(s)
#include <QMainWindow>

// Forward declaration(s)
class QCloseEvent;
class QStackedWidget;

namespace ub {

/**
 * Main application window.
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    /**
     * Initializes the application main window.
     */
    MainWindow();

    /**
     * Returns the stacked widget containing the central content of the main window.
     *
     * @return Main window content stacked widget
     */
    QStackedWidget *contentWidget();

signals:
    /** Emitted when the open menu action is triggered. */
    void openDatabase();

protected:
    /**
     * Intercepts the window closing event to save window state.
     *
     * @param[in] event window closing event
     */
    void closeEvent(QCloseEvent* event);

protected slots:
    /**
     * Displays information about the application.
     */
    void about();

private:
    /** Stacked content widget */
    QStackedWidget *content_;

    /**
     * Stores current windows settings, such as size, geometry, etc.
     */
    void saveSettings();

    /**
     * Restores saved window settings.
     */
    void restoreSettings();
};

} // ub namespace
