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

// Qt include(s)
#include <QtWidgets>

// UnderBudget include(s)
#include "ToolBar.hpp"
#include "icons.hpp"

namespace ub {

//--------------------------------------------------------------------------------------------------
ToolBar::ToolBar(QWidget * parent) : QToolBar(parent) {
    setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    setObjectName("MainToolBar");
    setFloatable(false);
    setMovable(false);

    auto addtrn = new QAction(Icon::add(), tr("&Transaction"), this);
    addtrn->setStatusTip(tr("Create a new transaction"));
    connect(addtrn, &QAction::triggered, this, &ToolBar::addTransaction);

    auto accounts = new QAction(Icon::accounts(), tr("&Accounts"), this);
    accounts->setStatusTip(tr("View accounts"));
    connect(accounts, &QAction::triggered, this, &ToolBar::viewAccounts);

    auto envelopes = new QAction(Icon::envelopes(), tr("&Envelopes"), this);
    envelopes->setStatusTip(tr("View envelopes"));
    connect(envelopes, &QAction::triggered, this, &ToolBar::viewEnvelopes);

    auto incomes = new QAction(Icon::incomes(), tr("&Incomes"), this);
    incomes->setStatusTip(tr("View budgeted incomes"));
    connect(incomes, &QAction::triggered, this, &ToolBar::viewBudgetedIncomes);

    auto expenses = new QAction(Icon::expenses(), tr("E&xpenses"), this);
    expenses->setStatusTip(tr("View budgeted expenses"));
    connect(expenses, &QAction::triggered, this, &ToolBar::viewBudgetedExpenses);

    auto reports = new QAction(Icon::reports(), tr("&Reports"), this);
    reports->setStatusTip(tr("View reports"));
    connect(reports, &QAction::triggered, this, &ToolBar::viewReports);

    addAction(addtrn);
    addSeparator();
    addAction(accounts);
    addAction(envelopes);
    addAction(incomes);
    addAction(expenses);
    addAction(reports);
}
}
