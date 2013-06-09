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

#ifndef BUDGETDETAILSFORM_HPP
#define BUDGETDETAILSFORM_HPP

// Qt include(s)
#include <QSharedPointer>
#include <QWidget>

// UnderBudget include(s)
#include "accounting/Money.hpp"
#include "budget/Budget.hpp"

// Forward declaration(s)
class QLineEdit;
class QUndoStack;

namespace ub {

// Forward declaration(s)
class BudgetingPeriodForm;
class MoneyEdit;

/**
 * Budget detail form widget.
 */
class BudgetDetailsForm : public QWidget
{
	Q_OBJECT

public:
	/**
	 * Constructs a new budget detail entry form.
	 *
	 * @param budget budget represented by this form
	 * @param stack  undoable command stack
	 * @param parent parent widget
	 */
	BudgetDetailsForm(QSharedPointer<Budget> budget,
		QUndoStack* stack, QWidget* parent = 0);

private slots:
	/**
	 * Updates the budget's name based on the user-entered text.
	 *
	 * @param[in] name new budget name from the text input
	 */
	void updateName(const QString& name);

	/**
	 * Updates the budget's initial balance based on the user-entered text.
	 *
	 * @param[in] amount new initial balance from the money input
	 */
	void updateInitialBalance(const Money& amount);

private:
	/** Budget being modified */
	QSharedPointer<Budget> budget;
	/** Budget name entry field */
	QLineEdit* nameField;
	/** Initial balance entry field */
	MoneyEdit* initialBalanceField;
	/** Budgeting period entry field */
	BudgetingPeriodForm* periodField;

	/** Undo stack for all commands */
	QUndoStack* undoStack;
};

}

#endif //BUDGETDETAILSFORM_HPP
