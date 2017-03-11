/*
 * Copyright 2017 Kyle Treubig
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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
