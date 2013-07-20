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

#ifndef MDIWINDOW_HPP
#define MDIWINDOW_HPP

// UnderBudget include(s)
#include "ui/MainWindow.hpp"

// Forward declaration(s)
class QMdiArea;
class QMdiSubWindow;
class QSignalMapper;

namespace ub
{

/**
 * A main window for the application in which each opened budget is displayed
 * as a sub-window of the primary window.
 */
class MdiWindow : public MainWindow
{
	Q_OBJECT

public:
	/**
	 * Initializes the main window.
	 */
	MdiWindow();

protected:
	/**
	 * Opens the budget defined in the given budget source. If the given
	 * source is invalid, of the budget could not be retreived from the source,
	 * no action is taken (no new sub-window).
	 *
	 * @param[in] source budget source
	 */
	void openBudget(QSharedPointer<BudgetSource> source);

	/**
	 * Returns the session belonging to the active sub-window, or 0 if
	 * there are no active sub-windows.
	 *
	 * @return active session, or 0 if no active session
	 */
	Session* activeSession() const;

	/**
	 * Intercepts the window closing event to prompt the
	 * user to finalize any pending operations before closing
	 * the application.
	 *
	 * @param[in] event window closing event
	 */
	void closeEvent(QCloseEvent* event);

private slots:
	/**
	 * Sets the active sub-window.
	 *
	 * @param[in] window new active sub-window
	 */
	void setActiveSubWindow(QWidget* window);

private:
	// Content widgets
	QMdiArea* mdiArea;
	QSignalMapper* windowMapper;

	/**
	 * Creates a new budget session not yet associated with any file.
	 */
	void newBudget();

	/**
	 * Updates the window menu as needed according to the currently
	 * open budget sessions
	 */
	void updateWindowMenu();

	/**
	 * Create a new session as a sub-window.
	 *
	 * @return newly created session
	 */
	Session* createSession();

	/**
	 * Searches for an existing session for the given budget source.
	 *
	 * @param[in] source budget source to be searched
	 * @return sub-window whose session is associated with the given
	 *         budget source, or 0 if no session exists
	 */
	QMdiSubWindow* findSession(const QSharedPointer<BudgetSource>& source) const;
};

}

#endif //MDIWINDOW_HPP
