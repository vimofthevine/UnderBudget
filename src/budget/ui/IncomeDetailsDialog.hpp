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
#include <budget/model/Income.hpp>

// Forward declaration(s)
class QAbstractButton;
class QDateEdit;
class QDialogButtonBox;
class QLineEdit;
class QWidget;

namespace ub {
namespace ledger {
// Forward declaration(s)
class Account;
class DoubleLineEdit;
} // ledger namespace

namespace budget {

// Forward declaration(s)
class IncomeModel;
class RecurrenceEditWidget;

/**
 * User input dialog for creating and modifying budgeted incomes.
 *
 * @ingroup budget
 */
class IncomeDetailsDialog : public QDialog {
    Q_OBJECT

public:
    /**
     * Initializes the income details dialog.
     *
     * @param model  Income model
     * @param parent Parent widget
     */
    IncomeDetailsDialog(IncomeModel * model, QWidget * parent);

public slots:
    /**
     * Updates the form to define a new budgeted income under the given account.
     *
     * @param account Account for which to create a new income
     */
    void resetForNewIncome(const ledger::Account & account);

    /**
     * Updates the form to modify an existing income.
     *
     * @param index Model index of the income to be modified
     */
    void showIncome(const QModelIndex & index);

    /**
     * Updates the form to duplicate an existing income.
     *
     * @param index Model index of the income to be duplicated
     */
    void duplicateIncome(const QModelIndex & index);

private slots:
    /**
     * Responds to a button click as appropriate.
     */
    void clicked(QAbstractButton * button);

private:
    /** Income model */
    IncomeModel * model_;
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

    /** Income model index */
    QModelIndex index_;
    /** Income shown in the form */
    Income income_;
};

} // budget namespace
} // ub namespace
