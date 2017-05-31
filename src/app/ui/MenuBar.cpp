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
#include "MenuBar.hpp"
#include "icons.hpp"

namespace ub {

//--------------------------------------------------------------------------------------------------
MenuBar::MenuBar(QWidget * parent) : QMenuBar(parent) {
    auto open = new QAction(Icon::openDocument(), tr("&Open..."), this);
    open->setShortcut(QKeySequence::Open);
    open->setStatusTip(tr("Open database"));
    connect(open, &QAction::triggered, this, &MenuBar::openDatabase);

    auto exit = new QAction(Icon::exit(), tr("E&xit"), this);
    exit->setShortcut(QKeySequence::Quit);
    exit->setStatusTip(tr("Quit the application"));
    connect(exit, &QAction::triggered, this, &MenuBar::exitApplication);

    auto addtrn = new QAction(Icon::add(), tr("&Add Transaction..."), this);
    addtrn->setShortcut(QKeySequence::New);
    addtrn->setStatusTip(tr("Create a new transaction"));
    connect(addtrn, &QAction::triggered, this, &MenuBar::addTransaction);

    auto accounts = new QAction(Icon::accounts(), tr("&Accounts"), this);
    accounts->setShortcut(QKeySequence(Qt::Key_F5));
    accounts->setStatusTip(tr("View accounts"));
    connect(accounts, &QAction::triggered, this, &MenuBar::viewAccounts);

    auto envelopes = new QAction(Icon::envelopes(), tr("&Envelopes"), this);
    envelopes->setShortcut(QKeySequence(Qt::Key_F6));
    envelopes->setStatusTip(tr("View envelopes"));
    connect(envelopes, &QAction::triggered, this, &MenuBar::viewEnvelopes);

    auto incomes = new QAction(Icon::incomes(), tr("Budgeted &Incomes"), this);
    incomes->setShortcut(QKeySequence(Qt::Key_F7));
    incomes->setStatusTip(tr("View budgeted incomes"));
    connect(incomes, &QAction::triggered, this, &MenuBar::viewBudgetedIncomes);

    auto expenses = new QAction(Icon::expenses(), tr("Budgeted E&xpenses"), this);
    expenses->setShortcut(QKeySequence(Qt::Key_F8));
    expenses->setStatusTip(tr("View budgeted expenses"));
    connect(expenses, &QAction::triggered, this, &MenuBar::viewBudgetedExpenses);

    auto reports = new QAction(Icon::reports(), tr("&Reports"), this);
    reports->setShortcut(QKeySequence(Qt::Key_F9));
    reports->setStatusTip(tr("View reports"));
    connect(reports, &QAction::triggered, this, &MenuBar::viewReports);

    auto about = new QAction(Icon::about(), tr("&About %1").arg(qApp->applicationName()), this);
    about->setStatusTip(tr("About the application"));
    connect(about, &QAction::triggered, this, &MenuBar::aboutApplication);

    auto aboutqt = new QAction(QIcon(":/icons/aboutQt"), tr("About &Qt"), this);
    aboutqt->setStatusTip(tr("About the Qt library"));
    connect(aboutqt, &QAction::triggered, this, &MenuBar::aboutQt);

    auto file = addMenu(tr("&File"));
    file->addAction(open);
    file->addSeparator();
    file->addAction(exit);

    auto edit = addMenu(tr("&Edit"));
    edit->addAction(addtrn);

    auto view = addMenu(tr("&View"));
    view->addAction(accounts);
    view->addAction(envelopes);
    view->addAction(incomes);
    view->addAction(expenses);
    view->addAction(reports);

    addSeparator();

    auto help = addMenu(tr("&Help"));
    help->addAction(about);
    help->addAction(aboutqt);
}

} // ub namespace
