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
#include <QToolBar>

// Forward declaration(s)
class QWidget;

namespace ub {

/**
 * Main application window toolbar.
 */
class ToolBar : public QToolBar {
    Q_OBJECT

public:
    /**
     * Initializes the toolbar.
     *
     * @param parent Parent widget
     */
    ToolBar(QWidget * parent);

signals:
    /** Emitted when the add-transaction menu action is triggered. */
    void addTransaction();

    /** Emitted when the view-accounts menu action is triggered. */
    void viewAccounts();

    /** Emitted when the view-envelopes menu action is triggered. */
    void viewEnvelopes();

    /** Emitted when the view-incomes menu action is triggered. */
    void viewBudgetedIncomes();

    /** Emitted when the view-expenses menu action is triggered. */
    void viewBudgetedExpenses();

    /** Emitted when the view-reports menu action is triggered. */
    void viewReports();
};
}