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

#ifndef SDIWINDOW_HPP
#define SDIWINDOW_HPP

// UnderBudget include(s)
#include "ui/MainWindow.hpp"

namespace ub
{

/**
 * A main window for the application in which each budget is displayed in its
 * own window.
 */
class SdiWindow : public MainWindow
{
	Q_OBJECT

public:
	/**
	 * Constructs a main window.
	 *
	 * @param[in] source optional source to be opened right away in the new window
	 */
	SdiWindow(QSharedPointer<BudgetSource> source = QSharedPointer<BudgetSource>());

private slots:
	/**
	 * Brings this window to focus, raising it above other main windows.
	 */
	void takeFocus();

	/**
	 * Closes this window. If it is the last open window, it will remain
	 * open but remove the open budget session.
	 *
	 * @param[in] keepOpen if `true`, closes the session but keeps the
	 *                     window open
	 * @return `true` if the window itself was closed
	 */
	bool closeWindow(bool keepOpen = false);

	/**
	 * Closes all open budget sessions and their respective windows.
	 */
	void closeAllWindows();

protected:
	/**
	 * Opens the budget defined in the given budget source. If the given
	 * source is invalid, of the budget could not be retreived from the source,
	 * no action is taken (no new session or windows).
	 *
	 * @param[in] source budget source
	 */
	void openBudget(QSharedPointer<BudgetSource> source);

	/**
	 * Returns the session for this window, or 0 if a session has not
	 * yet been created.
	 *
	 * @return active session, or 0 if no active session
	 */
	Session* activeSession() const;

	/**
	 * Intercepts the closing event to prompt the user to save pending
	 * changes.
	 *
	 * @param[in] event closing event
	 */
	void closeEvent(QCloseEvent* event);

private:
	// Content widgets
	Session* session;

	/**
	 * Constructs a main window with a new budget created right away.
	 */
	SdiWindow(bool);

	/**
	 * Creates a new budget session not yet associated with any file.
	 */
	void newBudget();

	/**
	 * Updates the window menu.
	 */
	void updateWindowMenu();

	/**
	 * Creates a new session for this window.
	 */
	void createSession();

	/**
	 * Searches for an existing window/session for the given budget source.
	 *
	 * @param[in] source budget source to be searched
	 * @return main window whose session is associated with the given
	 *         budget source, or 0 if no window exists
	 */
	SdiWindow* findMainWindow(const QSharedPointer<BudgetSource>& source) const;
};

}

#endif //SDIWINDOW_HPP
