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
class QMdiSubWindow;
class QProgressBar;
class QSignalMapper;

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
	 * Creates a new budget session for the user-specified file.
	 */
	void openBudget();

	/**
	 * Opens a recent budget file
	 */
	void openRecentBudget();

	/**
	 * Saves changes to the current budget
	 */
	void saveBudget();

	/**
	 * Saves the current budget to a new file
	 */
	void saveBudgetAs();

	/**
	 * Saves the current budget as the template budget
	 */
	void saveBudgetAsTemplate();

	/**
	 * Displays the edit-budget view in the current session
	 */
	void editBudget();

	/**
	 * Displays the edit-estimates view in the current session
	 */
	void editEstimates();

	/**
	 * Displays the edit-rules view in the current session
	 */
	void editAssignmentRules();

	/**
	 * Re-imports transactions into the current session
	 */
	void importTransactions();

	/**
	 * Imports transactions from a file into the current session
	 */
	void importTransactionsFrom();

	/**
	 * Assigns imported transactions in the current session
	 */
	void assignTransactions();

	/**
	 * Calculates ending balances in the current session
	 */
	void calculateBalances();

	/**
	 * Displays the analysis summary view in the current session
	 */
	void showAnalysisSummary();

	/**
	 * Displays the estimate progress view in the current session
	 */
	void showEstimateProgress();

	/**
	 * Displays the estimate impact view in the current session
	 */
	void showEstimateImpact();

	/**
	 * Displays the imported transactions view in the current session
	 */
	void showImportedTransactions();

	/**
	 * Displays the given message temporarily in the status bar
	 *
	 * @param[in] message temporary status message
	 */
	void showStatusMessage(const QString& message);

	/**
	 * Displays the specified progress in the status bar.
	 *
	 * If both value and max are 0, then an indefinite, or busy, progress
	 * will be displayed.
	 *
	 * If both value and max are the same, the progress bar will be
	 * hidden.
	 *
	 * @param[in] value percent-complete value
	 * @param[in] max   maximum value
	 */
	void showProgress(int value, int max);

	/**
	 * Updates the menu actions according to whether a budget session
	 * is currently open or active.
	 */
	void updateMenus();

	/**
	 * Updates the recent-files menu with a list of recently-opened files
	 */
	void updateRecentFilesMenu();

	/**
	 * Updates the window menu as needed according to the currently
	 * open budget sessions
	 */
	void updateWindowMenu();

	/**
	 * Sets the active sub-window.
	 *
	 * @param[in] window new active sub-window
	 */
	void setActiveSubWindow(QWidget* window);

	/**
	 * Displays information about the application.
	 */
	void about();

private:
	// Maximum number of recent budget files to remember
	static const int MAX_RECENT_BUDGET_FILES;
	// Recent budget files settings key
	static const QString RECENT_BUDGET_FILES;
	// Main window size settings key
	static const QString MAIN_WINDOW_SIZE;
	// Main window state settings key
	static const QString MAIN_WINDOW_STATE;

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
	QSignalMapper* windowMapper;
	QProgressBar* progressBar;

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

	/**
	 * Opens the budget defined in the specified file. If the given file name
	 * is blank, or if the file could not be opened, no action is taken.
	 *
	 * @param[in] fileName budget file name
	 */
	void openBudget(const QString fileName);

	/**
	 * Returns the active session, or 0 if no active session.
	 *
	 * @return active session, or 0 if no active session
	 */
	Session* activeSession() const;

	/**
	 * Create a new session as a sub-window.
	 *
	 * @return newly created session
	 */
	Session* createSession();

	/**
	 * Searches for an open session for the specified file.
	 *
	 * @param[in] file file name to be searched
	 * @return sub-window whose session is associated with the given
	 *         file, or 0 if no session exists
	 */
	QMdiSubWindow* findSession(const QString& file) const;

	/**
	 * Records the given file as a recent budget file, so it can
	 * be displayed in the recent-files menu.
	 *
	 * @param[in] file budget file name to be recorded
	 */
	void recordRecentBudget(const QString& file);
};

}

#endif //MAINWINDOW_HPP
