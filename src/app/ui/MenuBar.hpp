#pragma once

// Qt include(s)
#include <QMenuBar>

// Forward declaration(s)
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
    MenuBar(QWidget * parent);

signals:
    /** Emitted when the open menu action is triggered. */
    void openDatabase();

    /** Emitted when the exit menu action is triggered. */
    void exitApplication();

    /** Emitted when the add-transaction menu action is triggered. */
    void addTransaction();

    /** Emitted when the view-accounts menu action is triggered. */
    void viewAccounts();

    /** Emitted when the view-envelopes menu action is triggered. */
    void viewEnvelopes();

    /** Emitted when the about-app menu action is triggered. */
    void aboutApplication();

    /** Emitted when the about-Qt menu action is triggered. */
    void aboutQt();
};

} // ub namespace
