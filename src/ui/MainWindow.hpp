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

#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

// Qt include(s)
#include <QMainWindow>

// Forward declaration(s)
class QMdiArea;

namespace ub
{

// Forward declaration(s)
class Session;

/**
 * The main, primary application window.
 */
class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	/**
	 * Initializes the main window.
	 */
	MainWindow();

protected:
	/**
	 * Intercepts the window closing event to prompt the
	 * user to finalize any pending operations before closing
	 * the application.
	 *
	 * @param[in] event window closing event
	 */
	void closeEvent(QCloseEvent* event);

private slots:
	void notImpl();

	/**
	 * Creates a new budget session not yet associated with any file.
	 */
	void newBudget();

	/**
	 * Updates the menu actions according to whether a budget session
	 * is currently open or active.
	 */
	void updateMenus();

	/**
	 * Updates the window menu as needed according to the currently
	 * open budget sessions
	 */
	void updateWindowMenu();

	/**
	 * Displays information about the application.
	 */
	void about();

private:
	// File menu actions
	QAction* newAction;
	QAction* openAction;
	QAction* saveAction;
	QAction* saveAsAction;
	QAction* saveAsTemplateAction;
	QAction* exportAction;
	QAction* closeAction;
	QAction* closeAllAction;
	QAction* exitAction;

	// Edit menu actions
	QAction* undoAction;
	QAction* redoAction;
	QAction* editBudgetAction;
	QAction* editEstimatesAction;
	QAction* editRulesAction;
	QAction* editPrefsAction;

	// Analyze menu actions
	QAction* importAction;
	QAction* importFromAction;
	QAction* assignAction;
	QAction* calculateAction;
	QAction* summaryAction;
	QAction* progressAction;
	QAction* impactAction;
	QAction* transactionsAction;

	// Window menu actions
	QAction* tileAction;
	QAction* cascadeAction;

	// Help menu actions
	QAction* aboutAction;
	QAction* aboutQtAction;

	// Menus
	QMenu* fileMenu;
	QMenu* recentFilesMenu;
	QMenu* editMenu;
	QMenu* analyzeMenu;
	QMenu* windowMenu;
	QMenu* helpMenu;

	// Toolbars
	QToolBar* fileToolBar;
	QToolBar* editToolBar;
	QToolBar* analyzeToolBar;

	// Content widgets
	QMdiArea* mdiArea;

	/** Creates all actions */
	void createActions();

	/** Creates all menus */
	void createMenus();

	/** Creates all toolbars */
	void createToolBars();

	/** Creates the status bar */
	void createStatusBar();

	/** Store window settings such as size, geometry, etc.  */
	void writeSettings();

	/** Restore saved window settings.  */
	void readSettings();

	/** Create a new session as a sub-window */
	Session* createSession();
};

}

#endif //MAINWINDOW_HPP
