/*
 * UnderBudget
 * Copyright 2017 Kyle Treubig
 *
 * UnderBudget is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * UnderBudget is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with UnderBudget.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

// Qt include(s)
#include <QMainWindow>

// Forward declaration(s)
class QCloseEvent;
class QStackedWidget;

namespace ub {

// Forward declaration(s)
class MenuBar;
class ToolBar;

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
     * Returns the main window toolbar.
     *
     * @return Main window toolbar
     */
    ToolBar * toolBar() const;

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
    /** Toolbar */
    ToolBar * toolbar_;
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
