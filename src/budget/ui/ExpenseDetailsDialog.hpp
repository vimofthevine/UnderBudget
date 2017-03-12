/*
 * Copyright 2017 Kyle Treubig
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

#pragma once

// Qt include(s)
#include <QDialog>
#include <QModelIndex>

// UnderBudget include(s)
#include <budget/model/Expense.hpp>

// Forward declaration(s)
class QAbstractButton;
class QDateEdit;
class QDialogButtonBox;
class QLineEdit;
class QWidget;

namespace ub {
namespace ledger {
// Forward declaration(s)
class DoubleLineEdit;
class Envelope;
} // ledger namespace

namespace budget {

// Forward declaration(s)
class ExpenseModel;
class RecurrenceEditWidget;

/**
 * User input dialog for creating and modifying budgeted expenses.
 *
 * @ingroup budget
 */
class ExpenseDetailsDialog : public QDialog {
    Q_OBJECT

public:
    /**
     * Initializes the expense details dialog.
     *
     * @param model  Expense model
     * @param parent Parent widget
     */
    ExpenseDetailsDialog(ExpenseModel * model, QWidget * parent);

public slots:
    /**
     * Updates the form to define a new budgeted expense under the given envelope.
     *
     * @param envelope Envelope for which to create a new expense
     */
    void resetForNewExpense(const ledger::Envelope & envelope);

    /**
     * Updates the form to modify an existing expense.
     *
     * @param index Model index of the expense to be modified
     */
    void showExpense(const QModelIndex & index);

    /**
     * Updates the form to duplicate an existing expense.
     *
     * @param index Model index of the expense to be duplicated
     */
    void duplicateExpense(const QModelIndex & index);

private slots:
    /**
     * Responds to a button click as appropriate.
     */
    void clicked(QAbstractButton * button);

private:
    /** Expense model */
    ExpenseModel * model_;
    /** Description input widget */
    QLineEdit * description_;
    /** Beginning date input widget */
    QDateEdit * beginning_date_;
    /** Ending date input widget */
    QDateEdit * ending_date_;
    /** Recurrence input widget */
    RecurrenceEditWidget * recurrence_;
    /** Amount input widget */
    ledger::DoubleLineEdit * amount_;
    /** Button box */
    QDialogButtonBox * buttons_;

    /** Expense model index */
    QModelIndex index_;
    /** Expense shown in the form */
    Expense expense_;
};

} // budget namespace
} // ub namespace
