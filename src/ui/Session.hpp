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

#ifndef SESSION_HPP
#define SESSION_HPP

// Qt include(s)
#include <QStackedWidget>

namespace ub {

/**
 * Widget for an open budget session.
 */
class Session : public QStackedWidget
{
	Q_OBJECT

public:
	/**
	 * Constructs a new
	 */
	Session(QWidget* parent = 0);

	/**
	 * Creates a new budget file
	 */
	void newBudgetFile();

	/**
	 * Opens the budget defined in the specified file.
	 *
	 * @param[in] file budget file name
	 * @return `true` if the file was successfully opened
	 */
	bool openBudgetFile(const QString& file);

	/**
	 * Saves any modifications to the budget.
	 *
	 * @return `true` if the budget was saved, or `false` if the user
	 *         cancelled the save operation
	 */
	bool save();

	/**
	 * Saves this budget to a new file.
	 *
	 * @return `true` if the budget was saved, or `false` if the user
	 *         cancelled the save operation
	 */
	bool saveAs();

	/**
	 * Saves this budget as the template budget.
	 *
	 * @return `true` if the budget was saved, or `false` if the user
	 *         cancelled the save operation
	 */
	bool saveAsTemplate();

	/**
	 * Displays the edit-budget view.
	 */
	void editBudget();

	/**
	 * Displays the edit-estimates view.
	 */
	void editEstimates();

	/**
	 * Displays the edit-rules view.
	 */
	void editAssignmentRules();

	/**
	 * Imports transactions from the last-used transaction file.
	 */
	void importTransactions();

	/**
	 * Imports transactions from a user-specified transaction file.
	 */
	void importTransactionsFrom();

	/**
	 * Displays the analysis summary view.
	 */
	void showAnalysisSummary();

	/**
	 * Displays the estimate progress view.
	 */
	void showEstimateProgress();

	/**
	 * Displays the estimate impact view.
	 */
	void showEstimateImpact();

	/**
	 * Displays the imported transactions view.
	 */
	void showImportedTransactions();

	/**
	 * Assigns imported transactions.
	 */
	void assignTransactions();

	/**
	 * Calculates ending balances.
	 */
	void calculateBalances();

	/**
	 * Returns the name of the budget associated with this session.
	 *
	 * @return name of the current budget
	 */
	QString budgetName() const;

	/**
	 * Returns the name of the file in which the current budget
	 * is defined.
	 *
	 * @return name of the current budget file
	 */
	QString currentFileName() const;

	/**
	 * Checks if there are undo-able actions for this session.
	 *
	 * @return `true` if there are undo-able actions
	 */
	bool hasUndoableActions() const;

	/**
	 * Checks if there are redo-able actions for this session.
	 *
	 * @return `true` if there are redo-able actions
	 */
	bool hasRedoableActions() const;

signals:
	/**
	 * Emitted to display a temporary status message to the user.
	 *
	 * @param message status message
	 */
	void showMessage(const QString& message);

	/**
	 * Emitted to display progress of a particular task.
	 *
	 * @param value percent-complete value
	 * @param max   maximum value
	 */
	void showProgress(int value, int max);

	/**
	 * Emitted to indicate that there are recent actions that can
	 * be undone.
	 *
	 * @param available `true` if there are undo-able actions
	 */
	void undoAvailable(bool available);

	/**
	 * Emitted to indicate that there are recent actions that can
	 * be redone.
	 *
	 * @param available `true` if there are redo-able actions
	 */
	void redoAvailable(bool available);

protected:
	/**
	 * Intercepts the window closing event to prompt the
	 * user to finalize any pending operations before closing
	 * the session.
	 *
	 * @param[in] event window closing event
	 */
	void closeEvent(QCloseEvent* event);

private:
	/** Name of the current budget file */
	QString currentFile;
	/** Whether the current budget file is a new file */
	bool isUntitled;

	/**
	 * Saves the budget to the specified file.
	 *
	 * @param[in] file name of the file to which to save the budget
	 */
	bool save(const QString& file);
};

}

#endif //SESSION_HPP
