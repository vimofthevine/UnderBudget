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
#include <QSharedPointer>
#include <QStackedWidget>

// UnderBudget include(s)
#include "budget/Budget.hpp"
#include "budget/storage/BudgetSource.hpp"

// Forward declaration(s)
class QUndoStack;

namespace ub {

// Forward declaration(s)
class BudgetDetailsForm;
class EstimateDisplayWidget;
class RulesListWidget;

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
	 * Creates a new budget
	 */
	void newBudget();

	/**
	 * Opens the budget defined in the given budget source.
	 *
	 * @param[in] source budget source
	 * @return `true` if the budget was successfully retrieved
	 */
	bool openBudget(QSharedPointer<BudgetSource> source);

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
	 * Returns the name of this session, which is a combination of
	 * the budget name and source location.
	 *
	 * @return name of this session
	 */
	QString sessionName() const;

	/**
	 * Returns the name of the budget associated with this session.
	 *
	 * @return name of the current budget
	 */
	QString budgetName() const;

	/**
	 * Returns the budget source associated with this session.
	 *
	 * @return current budget source
	 */
	QSharedPointer<BudgetSource> currentBudgetSource() const;

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

	/**
	 * Undoes the previous modification action perfomed in this session, if any.
	 */
	void undo();

	/**
	 * Redoes the previously undone modification action performed in this
	 * session, if any.
	 */
	void redo();

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

private slots:
	/**
	 * Updates the session window's title as the session name plus a
	 * document-modified symbol.
	 */
	void updateWindowTitle();

	/**
	 * Sets the window modified flag based on the clean state of the undo stack.
	 * Since the `QStackedWidget::setWindowModified` slot must be set to the
	 * opposite of the `QUndoStack::cleanChanged` signal, they cannot be
	 * directly connected.
	 */
	void setWindowModified(bool isClean);

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
	/** Budget modification undo stack */
	QUndoStack* undoStack;
	/** Current budget source */
	QSharedPointer<BudgetSource> budgetSource;
	/** Current budget */
	QSharedPointer<Budget> budget;
	/** Whether the current budget is a new, unsaved budget */
	bool isUntitled;

	/** Budget details form */
	BudgetDetailsForm* budgetDetails;
	/** Estimate display widget */
	EstimateDisplayWidget* estimateDisplay;
	/** Assignment rules list widget */
	RulesListWidget* assignmentRules;

	/**
	 * Creates all display widgets for this session. A budget must have
	 * been created or provided to this session before performing this
	 * operation, as the budget is provided to the child views.
	 */
	void createWidgets();

	/**
	 * Prompts the user to save any unsaved changes. The user can
	 * choose to either save the changes, discard the changes, or
	 * cancel whatever operation has triggered this prompt.
	 *
	 * @return `true` if changes are saved or being discarded, or
	 *         `false` if the triggering operation should be cancelled
	 */
	bool promptToSave();

	/**
	 * Saves the budget to the specified source.
	 *
	 * @param[in] source budget source to which to save the budget
	 * @return `true` if the budget was saved successfully
	 */
	bool save(const QSharedPointer<BudgetSource>& source);
};

}

#endif //SESSION_HPP
