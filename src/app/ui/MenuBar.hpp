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

    /** Emitted when the view-expenses menu action is triggered. */
    void viewBudgetedExpenses();

    /** Emitted when the about-app menu action is triggered. */
    void aboutApplication();

    /** Emitted when the about-Qt menu action is triggered. */
    void aboutQt();
};

} // ub namespace
