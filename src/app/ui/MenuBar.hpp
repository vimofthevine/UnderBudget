#pragma once

// Forward declaration(s)
class QMenuBar;

namespace ub {

/**
 * Main application window menu bar.
 */
class MenuBar {
public:
    /**
     * Initializes the menu bar.
     *
     * @param menu Main window's menu bar
     */
    MenuBar(QMenuBar *menu);

private:
    // File menu actions

    /** Open-database action */
    QAction *open_;
    /** Exit application action */
    QAction *exit_;

    // Help menu actions

    /** About-UnderBudget action */
    QAction *about_;
    /** About-Qt action */
    QAction *about_qt_;
};

} // ub namespace
