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
#include "ui/Session.hpp"
#include "ui/icons.hpp"

namespace ub
{

//------------------------------------------------------------------------------
// This implementation file defines all MainWindow methods that define or update
// the menu or toolbars.
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void MainWindow::createActions()
{
	// File menu actions
	newAction = new QAction(Icon::newDocument(), tr("&New"), this);
	newAction->setPriority(QAction::LowPriority);
	newAction->setShortcuts(QKeySequence::New);
	newAction->setStatusTip(tr("Create a new budget"));
	connect(newAction, SIGNAL(triggered()), this, SLOT(newBudget()));

	openAction = new QAction(Icon::openDocument(), tr("&Open..."), this);
	openAction->setPriority(QAction::LowPriority);
	openAction->setShortcuts(QKeySequence::Open);
	openAction->setStatusTip(tr("Open an existing budget"));
	connect(openAction, SIGNAL(triggered()), this, SLOT(openBudget()));

	saveAction = new QAction(Icon::saveDocument(), tr("&Save"), this);
	saveAction->setPriority(QAction::LowPriority);
	saveAction->setShortcuts(QKeySequence::Save);
	saveAction->setStatusTip(tr("Save changes to the budget"));
	connect(saveAction, SIGNAL(triggered()), this, SLOT(saveBudget()));

	saveAsAction = new QAction(Icon::saveAs(), tr("Save &As..."), this);
	saveAsAction->setShortcuts(QKeySequence::SaveAs);
	saveAsAction->setStatusTip(tr("Save as a new budget"));
	connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveBudgetAs()));

	saveAsTemplateAction = new QAction(Icon::saveAsTemplate(), tr("Save As &Template..."), this);
	saveAsTemplateAction->setStatusTip(tr("Save as template for new budgets"));
	connect(saveAsTemplateAction, SIGNAL(triggered()), this, SLOT(saveBudgetAsTemplate()));

	exportAction = new QAction(Icon::exportFile(), tr("&Export..."), this);
	exportAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_X));
	exportAction->setStatusTip(tr("Export analysis results"));
	connect(exportAction, SIGNAL(triggered()), this, SLOT(notImpl()));

	closeAction = new QAction(Icon::close(), tr("&Close"), this);
	closeAction->setShortcuts(QKeySequence::Close);
	closeAction->setShortcutContext(Qt::WidgetShortcut);
	closeAction->setStatusTip(tr("Close the budget"));

	closeAllAction = new QAction(Icon::closeAll(), tr("Close A&ll"), this);
	closeAllAction->setStatusTip(tr("Close all budgets"));

	exitAction = new QAction(Icon::exit(), tr("E&xit"), this);
	exitAction->setShortcuts(QKeySequence::Quit);
	exitAction->setStatusTip(tr("Quit the application"));
	connect(exitAction, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));

	// Edit menu actions
	undoAction = new QAction(Icon::undo(), tr("&Undo"), this);
	undoAction->setShortcut(QKeySequence::Undo);
	undoAction->setStatusTip(tr("Undo the last edit"));
	connect(undoAction, SIGNAL(triggered()), this, SLOT(undo()));

	redoAction = new QAction(Icon::redo(), tr("Re&do"), this);
	redoAction->setShortcut(QKeySequence::Redo);
	redoAction->setStatusTip(tr("Re-apply the last undone edit"));
	connect(redoAction, SIGNAL(triggered()), this, SLOT(redo()));

	editBudgetAction = new QAction(Icon::editBudget(), tr("&Budget"), this);
	editBudgetAction->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_B));
	editBudgetAction->setStatusTip(tr("Edit budget"));
	connect(editBudgetAction, SIGNAL(triggered()), this, SLOT(editBudget()));

	editEstimatesAction = new QAction(Icon::editEstimates(), tr("&Estimates"), this);
	editEstimatesAction->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_E));
	editEstimatesAction->setStatusTip(tr("Edit estimates"));
	connect(editEstimatesAction, SIGNAL(triggered()), this, SLOT(editEstimates()));

	editRulesAction = new QAction(Icon::editRules(), tr("Assignment &Rules"), this);
	editRulesAction->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_R));
	editRulesAction->setIconText(tr("Rules"));
	editRulesAction->setStatusTip(tr("Edit assignment rules"));
	connect(editRulesAction, SIGNAL(triggered()), this, SLOT(editAssignmentRules()));

	editPrefsAction = new QAction(Icon::editPreferences(), tr("&Preferences"), this);
	editPrefsAction->setShortcuts(QKeySequence::Preferences);
	editPrefsAction->setStatusTip(tr("Edit application preferences"));
	connect(editPrefsAction, SIGNAL(triggered()), this, SLOT(editPreferences()));

	// Analyze menu actions
	importAction = new QAction(Icon::importTransactions(), tr("&Import transactions..."), this);
	importAction->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_I));
	importAction->setIconText(tr("Import"));
	importAction->setStatusTip(tr("Import transactions from last-used file"));
	connect(importAction, SIGNAL(triggered()), this, SLOT(importTransactions()));

	importFromAction = new QAction(Icon::importTransactionsFrom(), tr("Import transactions &from..."), this);
	importFromAction->setStatusTip(tr("Import transactions from file"));
	connect(importFromAction, SIGNAL(triggered()), this, SLOT(importTransactionsFrom()));

	assignAction = new QAction(Icon::assign(), tr("&Assign transactions"), this);
	assignAction->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_A));
	assignAction->setIconText(tr("Assign"));
	assignAction->setStatusTip(tr("Assign imported transactions"));
	connect(assignAction, SIGNAL(triggered()), this, SLOT(assignTransactions()));

	calculateAction = new QAction(Icon::calculate(), tr("&Calculate balances"), this);
	calculateAction->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_C));
	calculateAction->setIconText(tr("Calculate"));
	calculateAction->setStatusTip(tr("Calculate ending balances"));
	connect(calculateAction, SIGNAL(triggered()), this, SLOT(calculateBalances()));

	summaryAction = new QAction(Icon::analysisSummary(), tr("Anal&ysis summary"), this);
	summaryAction->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_Y));
	summaryAction->setIconText(tr("Summary"));
	summaryAction->setStatusTip(tr("Analysis summary"));
	connect(summaryAction, SIGNAL(triggered()), this, SLOT(showAnalysisSummary()));

	progressAction = new QAction(Icon::estimateProgress(), tr("Estimate &progress"), this);
	progressAction->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_P));
	progressAction->setIconText(tr("Progress"));
	progressAction->setStatusTip(tr("Estimate progress"));
	connect(progressAction, SIGNAL(triggered()), this, SLOT(showEstimateProgress()));

	impactAction = new QAction(Icon::estimateImpact(), tr("&Balance impact"), this);
	impactAction->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_L));
	impactAction->setIconText(tr("Impact"));
	impactAction->setStatusTip(tr("Balance impact"));
	connect(impactAction, SIGNAL(triggered()), this, SLOT(showEstimateImpact()));

	transactionsAction = new QAction(Icon::importedTransactions(), tr("Impor&ted transactions"), this);
	transactionsAction->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_T));
	transactionsAction->setIconText(tr("Transactions"));
	transactionsAction->setStatusTip(tr("View imported transactions"));
	connect(transactionsAction, SIGNAL(triggered()), this, SLOT(showImportedTransactions()));

	// Window menu actions
	tileAction = new QAction(Icon::windowTile(), tr("&Tile"), this);
	tileAction->setStatusTip(tr("Tile all open budgets"));

	cascadeAction = new QAction(Icon::windowCascade(), tr("&Cascade"), this);
	cascadeAction->setStatusTip("Cascade all open budgets");

	// Help menu actions
	aboutAction = new QAction(Icon::about(),
		tr("&About %1").arg(qApp->applicationName()), this);
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
	updateRecentFilesMenu();
	connect(recentFilesMenu, SIGNAL(aboutToShow()),
		this, SLOT(updateRecentFilesMenu()));

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
	editMenu->addAction(undoAction);
	editMenu->addAction(redoAction);
	editMenu->addSeparator();
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
	connect(windowMenu, SIGNAL(aboutToShow()),
		this, SLOT(updateWindowMenu()));

	menuBar()->addSeparator();

	helpMenu = menuBar()->addMenu(tr("&Help"));
	helpMenu->addAction(aboutAction);
	helpMenu->addAction(aboutQtAction);
}

//------------------------------------------------------------------------------
void MainWindow::createToolBars()
{
	mainToolBar = addToolBar(tr("Quick Actions"));
	mainToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
	mainToolBar->setObjectName("MainToolBar");
	mainToolBar->setFloatable(false);
	mainToolBar->setMovable(false);

	mainToolBar->addAction(newAction);
	mainToolBar->addAction(openAction);
	mainToolBar->addAction(saveAction);
	mainToolBar->addSeparator();
	mainToolBar->addAction(editBudgetAction);
	mainToolBar->addAction(editEstimatesAction);
	mainToolBar->addAction(editRulesAction);
	mainToolBar->addSeparator();
	mainToolBar->addAction(importAction);
	mainToolBar->addAction(assignAction);
	mainToolBar->addAction(calculateAction);
	mainToolBar->addSeparator();
	mainToolBar->addAction(summaryAction);
	mainToolBar->addAction(progressAction);
	mainToolBar->addAction(transactionsAction);
}

//------------------------------------------------------------------------------
void MainWindow::updateRecentFilesMenu()
{
	recentFilesMenu->clear();

	QSettings settings;
	QStringList recentFiles = settings.value(RECENT_BUDGET_FILES).toStringList();
	int numRecentFiles = qMin(recentFiles.size(), MAX_RECENT_BUDGET_FILES);

	for (int i=0; i<numRecentFiles; ++i)
	{
		QString fileName = QFileInfo(recentFiles[i]).fileName();
		QString text;
		if (i < 9)
		{
			text = tr("&%1 %2").arg(i+1).arg(fileName);
		}
		else
		{
			text = tr("%1 %2").arg(i+1).arg(fileName);
		}

		QAction* action = recentFilesMenu->addAction(text);
		action->setData(recentFiles[i]);
		connect(action, SIGNAL(triggered()), this, SLOT(openRecentBudget()));
	}
}

//------------------------------------------------------------------------------
void MainWindow::updateMenus()
{
	Session* session = activeSession();
	bool hasActiveSession = (session != 0);

	// File menu actions
	saveAction->setEnabled(hasActiveSession);
	saveAsAction->setEnabled(hasActiveSession);
	saveAsTemplateAction->setEnabled(hasActiveSession);
	exportAction->setEnabled(hasActiveSession);
	closeAction->setEnabled(hasActiveSession);
	closeAllAction->setEnabled(hasActiveSession);
	// Edit menu actions
	undoAction->setEnabled(hasActiveSession && session->hasUndoableActions());
	redoAction->setEnabled(hasActiveSession && session->hasRedoableActions());
	editBudgetAction->setEnabled(hasActiveSession);
	editEstimatesAction->setEnabled(hasActiveSession);
	editRulesAction->setEnabled(hasActiveSession);
	// Analyze menu actions
	importAction->setEnabled(hasActiveSession);
	importFromAction->setEnabled(hasActiveSession);
	assignAction->setEnabled(hasActiveSession);
	calculateAction->setEnabled(hasActiveSession);
	summaryAction->setEnabled(hasActiveSession);
	progressAction->setEnabled(hasActiveSession);
	impactAction->setEnabled(hasActiveSession);
	transactionsAction->setEnabled(hasActiveSession);
	// Window menu actions
	tileAction->setEnabled(hasActiveSession);
	cascadeAction->setEnabled(hasActiveSession);
}

}
