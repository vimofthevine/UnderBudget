#pragma once

// Qt include(s)
#include <QMainWindow>

// Forward declaration(s)
class QCloseEvent;
class QStackedWidget;

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

    /**
     * Returns the main window menu bar.
     *
     * @return Main window menu bar
     */
    MenuBar * menu() const;

    /**
     * Returns the stacked widget containing the central content of the main window.
     *
     * @return Main window content stacked widget
     */
    QStackedWidget * contentWidget() const;

protected:
    /**
     * Intercepts the window closing event to save window state.
     *
     * @param[in] event window closing event
     */
    void closeEvent(QCloseEvent * event);

protected slots:
    /**
     * Displays information about the application.
     */
    void about();

private:
    /** Menu bar */
    MenuBar * menu_;
    /** Stacked content widget */
    QStackedWidget * content_;

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
