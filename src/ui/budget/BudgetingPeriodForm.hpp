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

#ifndef BUDGETINGPERIODFORM_HPP
#define BUDGETINGPERIODFORM_HPP

// Qt include(s)
#include <QSharedPointer>
#include <QWidget>

// UnderBudget include(s)
#include "budget/BudgetingPeriod.hpp"

// Forward declaration(s)
class QComboBox;
class QDateEdit;
class QSpinBox;
class QStackedWidget;
class QTextEdit;
class QUndoStack;

namespace ub {

/**
 * Budgeting period input form widget.
 */
class BudgetingPeriodForm : public QWidget
{
	Q_OBJECT

public:
	/**
	 * Constructs a new budgeting period entry form.
	 *
	 * @param period budgeting period being edited by this form
	 * @param stack  undoable command stack
	 * @param parent parent widget
	 */
	BudgetingPeriodForm(QSharedPointer<BudgetingPeriod> period,
		QUndoStack* stack, QWidget* parent = 0);

private slots:
	/**
	 * Updates the widget's display and fields according to the
	 * new period parameters.
	 *
	 * @param[in] params new budgeting period definition parameters
	 */
	void updateWidgets(const BudgetingPeriod::Parameters& params);

	/**
	 * Updates the budgeting period definition parameters based
	 * on the current input widgets.
	 */
	void updateParams();

private:
	/** Undo stack for all commands */
	QUndoStack* undoStack;
	/** Budgeting period being modified */
	QSharedPointer<BudgetingPeriod> period;
	/** Flag to prevent modification of the period while setting fields */
	bool disableSignals;

	/** Period range display field */
	QLineEdit* rangeDisplay;
	/** Period type selection field */
	QComboBox* typeField;
	/** Period description display field */
	QTextEdit* description;
	/** Year selection field */
	QSpinBox* yearField;
	/** Month selection field */
	QComboBox* monthField;
	/** Custom range form */
	QWidget* customRangeForm;
	/** Start date selection field */
	QDateEdit* startDateField;
	/** End date selection field */
	QDateEdit* endDateField;

	/**
	 * Creates a string to display the current date range of the
	 * budgeting period (i.e., "M/D/YY to M/D/YY").
	 *
	 * @return budgeting period date range text
	 */
	QString createRangeText() const;

	/**
	 * Returns a description for the specified budgeting period type.
	 *
	 * @param[in] type budgeting period type
	 * @return budgeting period type description
	 */
	const QString& typeDescription(const BudgetingPeriod::Type& type);

	/**
	 * Shows or hides widgets as needed according to the given
	 * budgeting period type.
	 *
	 * @param[in] type budgeting period type
	 */
	void displayWidgets(const BudgetingPeriod::Type& type);
};

}

#endif //BUDGETINGPERIODFORM_HPP
