#pragma once

// Qt include(s)
#include <QMainWindow>

namespace ub {

// Forward declaration(s)
class MenuBar;

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
    /** Main menu bar */
    MenuBar *menu_;

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
