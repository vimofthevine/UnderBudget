/*
 * Copyright 2013 Kyle Treubig
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

// Qt include(s)
#include <QtWidgets>

// UnderBudget include(s)
#include "ui/MainWindow.hpp"

namespace ub
{

//------------------------------------------------------------------------------
void MainWindow::createActions()
{
	// File menu actions
	newAction = new QAction(QIcon(":/icons/new"), tr("&New"), this);
	newAction->setShortcuts(QKeySequence::New);
	newAction->setStatusTip(tr("Create a new budget"));
	connect(newAction, SIGNAL(triggered()), this, SLOT(notImpl()));

	openAction = new QAction(QIcon(":/icons/open"), tr("&Open..."), this);
	openAction->setShortcuts(QKeySequence::Open);
	openAction->setStatusTip(tr("Open an existing budget"));
	connect(openAction, SIGNAL(triggered()), this, SLOT(notImpl()));

	saveAction = new QAction(QIcon(":/icons/save"), tr("&Save"), this);
	saveAction->setShortcuts(QKeySequence::Save);
	saveAction->setStatusTip(tr("Save changes to the budget"));
	connect(saveAction, SIGNAL(triggered()), this, SLOT(notImpl()));

	saveAsAction = new QAction(QIcon(":/icons/saveAs"), tr("Save &As..."), this);
	saveAsAction->setShortcuts(QKeySequence::SaveAs);
	saveAsAction->setStatusTip(tr("Save as a new budget"));
	connect(saveAsAction, SIGNAL(triggered()), this, SLOT(notImpl()));

	saveAsTemplateAction = new QAction(QIcon(":/icons/saveAsTemplate"), tr("Save As &Template..."), this);
	saveAsTemplateAction->setStatusTip(tr("Save as template for new budgets"));
	connect(saveAsTemplateAction, SIGNAL(triggered()), this, SLOT(notImpl()));

	exportAction = new QAction(QIcon(":/icons/export"), tr("&Export..."), this);
	exportAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_X));
	exportAction->setStatusTip(tr("Export analysis results"));
	connect(exportAction, SIGNAL(triggered()), this, SLOT(notImpl()));

	closeAction = new QAction(QIcon(":/icons/close"), tr("&Close"), this);
	closeAction->setShortcuts(QKeySequence::Close);
	closeAction->setStatusTip(tr("Close the budget"));
	connect(closeAction, SIGNAL(triggered()), this, SLOT(notImpl()));

	closeAllAction = new QAction(QIcon(":/icons/closeAll"), tr("Close A&ll"), this);
	closeAllAction->setStatusTip(tr("Close all budgets"));
	connect(closeAllAction, SIGNAL(triggered()), this, SLOT(notImpl()));

	exitAction = new QAction(QIcon(":/icons/exit"), tr("E&xit"), this);
	exitAction->setShortcuts(QKeySequence::Quit);
	exitAction->setStatusTip(tr("Quit the application"));
	connect(exitAction, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));

	// Edit menu actions
	editBudgetAction = new QAction(QIcon(":/icons/editBudget"), tr("&Budget"), this);
	editBudgetAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_B));
	editBudgetAction->setStatusTip(tr("Edit budget"));
	connect(editBudgetAction, SIGNAL(triggered()), this, SLOT(notImpl()));

	editEstimatesAction = new QAction(QIcon(":/icons/editEstimates"), tr("&Estimates"), this);
	editEstimatesAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_E));
	editEstimatesAction->setStatusTip(tr("Edit estimates"));
	connect(editEstimatesAction, SIGNAL(triggered()), this, SLOT(notImpl()));

	editRulesAction = new QAction(QIcon(":/icons/editRules"), tr("Assignment &Rules"), this);
	editRulesAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_R));
	editRulesAction->setStatusTip(tr("Edit assignment rules"));
	connect(editRulesAction, SIGNAL(triggered()), this, SLOT(notImpl()));

	editPrefsAction = new QAction(QIcon(":/icons/editPrefs"), tr("&Preferences"), this);
	editPrefsAction->setShortcuts(QKeySequence::Preferences);
	editPrefsAction->setStatusTip(tr("Edit application preferences"));
	connect(editPrefsAction, SIGNAL(triggered()), this, SLOT(notImpl()));

	// Analyze menu actions
	importAction = new QAction(QIcon(":/icons/import"), tr("&Import transactions..."), this);
	importAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_I));
	importAction->setStatusTip(tr("Import transactions from last-used file"));
	connect(importAction, SIGNAL(triggered()), this, SLOT(notImpl()));

	importFromAction = new QAction(QIcon(":/icons/importFrom"), tr("Import transactions &from..."), this);
	importFromAction->setStatusTip(tr("Import transactions from file"));
	connect(importFromAction, SIGNAL(triggered()), this, SLOT(notImpl()));

	assignAction = new QAction(QIcon(":/icons/assign"), tr("&Assign transactions"), this);
	assignAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_A));
	assignAction->setStatusTip(tr("Assign imported transactions"));
	connect(assignAction, SIGNAL(triggered()), this, SLOT(notImpl()));

	calculateAction = new QAction(QIcon(":/icons/calculate"), tr("&Calculate balances"), this);
	calculateAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_C));
	calculateAction->setStatusTip(tr("Calculate ending balances"));
	connect(calculateAction, SIGNAL(triggered()), this, SLOT(notImpl()));

	summaryAction = new QAction(QIcon(":/icons/summary"), tr("Anal&ysis summary"), this);
	summaryAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Y));
	summaryAction->setStatusTip(tr("Analysis summary"));
	connect(summaryAction, SIGNAL(triggered()), this, SLOT(notImpl()));

	progressAction = new QAction(QIcon(":/icons/progress"), tr("Estimate &progress"), this);
	progressAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_P));
	progressAction->setStatusTip(tr("Estimate progress"));
	connect(progressAction, SIGNAL(triggered()), this, SLOT(notImpl()));

	impactAction = new QAction(QIcon(":/icons/impact"), tr("&Balance impact"), this);
	impactAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_L));
	impactAction->setStatusTip(tr("Balance impact"));
	connect(impactAction, SIGNAL(triggered()), this, SLOT(notImpl()));

	transactionsAction = new QAction(QIcon(":/icons/transactions"), tr("Impor&ted transactions"), this);
	transactionsAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_T));
	transactionsAction->setStatusTip(tr("View imported transactions"));
	connect(transactionsAction, SIGNAL(triggered()), this, SLOT(notImpl()));

	// Help menu actions
	aboutAction = new QAction(QIcon(":/icons/about"), tr("&About"), this);
	aboutAction->setStatusTip(tr("About the application"));
	connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));

	aboutQtAction = new QAction(QIcon(":/icons/aboutQt"), tr("About &Qt"), this);
	aboutQtAction->setStatusTip(tr("About the Qt library"));
	connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

//------------------------------------------------------------------------------
void MainWindow::createMenus()
{
	fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(newAction);
	fileMenu->addAction(openAction);

	recentFilesMenu = fileMenu->addMenu(tr("&Recent..."));
	//TODO uncomment this
	//updateRecentFilesMenu();
	//connect(recentFilesMenu, SIGNAL(aboutToShow()),
	//this, SLOT(updateRecentFilesMenu()));

	fileMenu->addSeparator();
	fileMenu->addAction(saveAction);
	fileMenu->addAction(saveAsAction);
	fileMenu->addAction(saveAsTemplateAction);
	fileMenu->addSeparator();
	fileMenu->addAction(exportAction);
	fileMenu->addSeparator();
	fileMenu->addAction(closeAction);
	fileMenu->addAction(closeAllAction);
	fileMenu->addAction(exitAction);

	editMenu = menuBar()->addMenu(tr("&Edit"));
	editMenu->addAction(editBudgetAction);
	editMenu->addAction(editEstimatesAction);
	editMenu->addAction(editRulesAction);
	editMenu->addSeparator();
	editMenu->addAction(editPrefsAction);

	analyzeMenu = menuBar()->addMenu(tr("&Analyze"));
	analyzeMenu->addAction(importAction);
	analyzeMenu->addAction(importFromAction);
	analyzeMenu->addAction(assignAction);
	analyzeMenu->addAction(calculateAction);
	analyzeMenu->addSeparator();
	analyzeMenu->addAction(summaryAction);
	analyzeMenu->addAction(progressAction);
	analyzeMenu->addAction(impactAction);
	analyzeMenu->addAction(transactionsAction);

	windowMenu = menuBar()->addMenu(tr("&Window"));
	//TODO uncomment this
	//updateWindowMenu();
	//connect(windowMenu, SIGNAL(aboutToShow()), this, SLOT(updateWindowMenu()));

	menuBar()->addSeparator();

	helpMenu = menuBar()->addMenu(tr("&Help"));
	helpMenu->addAction(aboutAction);
	helpMenu->addAction(aboutQtAction);
}

//------------------------------------------------------------------------------
void MainWindow::createToolBars()
{
	fileToolBar = addToolBar(tr("File"));
	fileToolBar->setFloatable(false);
	fileToolBar->setMovable(false);
	fileToolBar->addAction(newAction);
	fileToolBar->addAction(openAction);
	fileToolBar->addAction(saveAction);

	editToolBar = addToolBar(tr("Edit"));
	editToolBar->setFloatable(false);
	editToolBar->setMovable(false);
	editToolBar->addAction(editBudgetAction);
	editToolBar->addAction(editEstimatesAction);
	editToolBar->addAction(editRulesAction);

	analyzeToolBar = addToolBar(tr("Analyze"));
	analyzeToolBar->setFloatable(false);
	analyzeToolBar->setMovable(false);
	analyzeToolBar->addAction(importAction);
	analyzeToolBar->addAction(assignAction);
	analyzeToolBar->addAction(calculateAction);
	analyzeToolBar->addAction(summaryAction);
	analyzeToolBar->addAction(progressAction);
	analyzeToolBar->addAction(impactAction);
	analyzeToolBar->addAction(transactionsAction);
}

}
