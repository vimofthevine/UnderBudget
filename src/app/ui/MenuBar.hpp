#pragma once

// Qt include(s)
#include <QMenuBar>

// Forward declaration(s)
class QAction;
class QWidget;

namespace ub {

/**
 * Main application window menu bar.
 */
class MenuBar : public QMenuBar {
    Q_OBJECT

public:
    /**
     * Initializes the menu bar.
     *
     * @param parent Parent widget
     */
    MenuBar(QWidget *parent);

signals:
    /** Emitted when the open menu action is triggered. */
    void openDatabase();

    /** Emitted when the exit menu action is triggered. */
    void exitApplication();

    /** Emitted when the about-app menu action is triggered. */
    void aboutApplication();

    /** Emitted when the about-Qt menu action is triggered. */
    void aboutQt();

private:
    // File menu actions

    /** Open-database action */
    QAction *open_;
    /** Exit application action */
    QAction *exit_;

    // Help menu actions

    /** About-application action */
    QAction *about_;
    /** About-Qt action */
    QAction *about_qt_;
};

} // ub namespace
