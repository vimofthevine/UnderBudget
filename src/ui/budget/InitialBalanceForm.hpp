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

#ifndef INITIALBALANCEFORM_HPP
#define INITIALBALANCEFORM_HPP

// Qt include(s)
#include <QIcon>
#include <QSharedPointer>
#include <QWidget>

// UnderBudget include(s)
#include "budget/Balance.hpp"

// Forward declaration(s)
class QPushButton;

namespace ub {

// Forward declaration(s)
class ContributorsListWidget;
class Money;
class MoneyEdit;

/**
 * Initial balance input form widget.
 *
 * @ingroup ui_budget
 */
class InitialBalanceForm : public QWidget
{
	Q_OBJECT

public:
	/**
	 * Constructs a new initial balance entry form.
	 *
	 * @param[in] balance initial balance being edited by this form
	 * @param[in] stack   undoable command stack
	 * @param[in] parent  parent widget
	 */
	InitialBalanceForm(QSharedPointer<Balance> balance,
		QUndoStack* stack, QWidget* parent = 0);

private slots:
	/**
	 * Updates the initial balance based on the user-entered value.
	 *
	 * @param[in] value new initial balance value
	 */
	void updateValue(const Money& value);

	/**
	 * Updates the net value field.
	 */
	void updateDisplayedValue();

	/**
	 * Toggles the display of advanced input fields.
	 */
	void showAdvanced(bool show);

private:
	/** Undo stack for all commands */
	QUndoStack* undoStack;
	/** Balance being modified */
	QSharedPointer<Balance> balance;

	/** Money entry field */
	MoneyEdit* valueField;
	/** Show-advanced button */
	QPushButton* toggleButton;
	/** Add-contributor button */
	QPushButton* addButton;
	/** Delete-contributor button */
	QPushButton* deleteButton;
	/** Expand icon */
	QIcon expand;
	/** Collapse icon */
	QIcon collapse;
	/** Balance contributors widget */
	ContributorsListWidget* contributors;
};

}

#endif //INITIALBALANCEFORM_HPP
