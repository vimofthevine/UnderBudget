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

#ifndef ESTIMATEDETAILSFORM_HPP
#define ESTIMATEDETAILSFORM_HPP

// Qt include(s)
#include <QPersistentModelIndex>
#include <QWidget>

// Forward declaration(s)
class QCheckBox;
class QComboBox;
class QDateEdit;
class QLineEdit;
class QModelIndex;
class QPushButton;
class QUndoStack;

namespace ub {

// Forward declaration(s)
class EstimateModel;
class Money;
class MoneyEdit;

/**
 * Estimate detail input form widget.
 *
 * @ingroup ui_budget
 */
class EstimateDetailsForm : public QWidget
{
	Q_OBJECT

public:
	/**
	 * Constructs a new estimate detail entry form.
	 *
	 * @param[in] model  estimate tree model
	 * @param[in] stack  undoable command stack
	 * @param[in] parent parent widget
	 */
	EstimateDetailsForm(EstimateModel* model, QUndoStack* stack,
		QWidget* parent = 0);

public slots:
	/**
	 * Updates the entry form to reflect the newly selected estimate.
	 *
	 * @param[in] current  currently selected estimate index
	 * @param[in] previous previously selected estimate index
	 */
	void estimateSelected(const QModelIndex& current, const QModelIndex& previous);

	/**
	 * Checks if the currently displayed estimate is within the range
	 * of changed estimates, and updates the displayed fields accordingly.
	 *
	 * @param[in] topLeft     top left index of changed data
	 * @param[in] bottomRight bottom right index of changed data
	 */
	void dataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight);

private slots:
	/**
	 * Clears the due date entry field.
	 */
	void clearDueDate();

	/**
	 * Updates the current estimate's name based on the user-entered text.
	 *
	 * @param[in] name new estimate name from the text input
	 */
	void updateName(const QString& name);

	/**
	 * Updates the current estimate's description based on the user-entered text.
	 *
	 * @param[in] descrip new estimate description from the text input
	 */
	void updateDescription(const QString& desription);

	/**
	 * Updates the current estimate's type based on the type selection.
	 *
	 * @param[in] typeIndex new estimate type selection index
	 */
	void updateType(int typeIndex);

	/**
	 * Updates the current estimate's estimated amount based on the user-entered value.
	 *
	 * @param[in] amount new estimated amount from the money input
	 */
	void updateAmount(const Money& amount);

	/**
	 * Updates the current estimate's due date based on the user-entered value.
	 *
	 * @param[in] date new due date from the date input
	 */
	void updateDueDate(const QDate& date);

	/**
	 * Updates the current estimate's finished state based on the user selection.
	 *
	 * @param[in] finished new finished state from the check box
	 */
	void updateFinishedState(bool finished);

private:
	/** Estimate name entry field */
	QLineEdit* nameField;
	/** Estimate description entry field */
	QLineEdit* descriptionField;
	/** Estimate type entry field */
	QComboBox* typeField;
	/** Estimated amount entry field */
	MoneyEdit* amountField;
	/** Due date entry field */
	QDateEdit* dueDateField;
	/** Clear due date button */
	QPushButton* clearDueDateButton;
	/** Finished entry field */
	QCheckBox* finishedField;

	/** Estimate tree model */
	EstimateModel* model;
	/** Currently selected estimate index */
	QPersistentModelIndex currentIndex;
	/** Undo stack for all commands */
	QUndoStack* undoStack;

	/**
	 * Populates the fields with values from the current estimate.
	 *
	 * @param[in] index current estimate index
	 */
	void populate(const QModelIndex& index);
};

}

#endif //ESTIMATEDETAILSFORM_HPP
