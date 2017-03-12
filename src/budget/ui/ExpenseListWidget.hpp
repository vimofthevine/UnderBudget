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
#include <QSplitter>

// Forward declaration(s)
class QWidget;

// UnderBudget include(s)
#include <ledger/model/Envelope.hpp>

namespace ub {

namespace ledger {
// Forward declaration(s)
class EnvelopeDetailsDialog;
class EnvelopeModel;
class TreeView;
} // ledger namespace

namespace budget {
// Forward declaration(s)
class BudgetTableView;
class ExpenseDetailsDialog;
class ExpenseModel;

/**
 * Widget for displaying a list of envelopes and associated budgeted expenses.
 *
 * @ingroup budget
 */
class ExpenseListWidget : public QSplitter {
    Q_OBJECT

public:
    /**
     * Initializes the expense list widget.
     *
     * @param[in] envelopes Envelope model
     * @param[in] expenses  Budgeted expenses model
     * @param[in] parent    Parent widget
     */
    ExpenseListWidget(ledger::EnvelopeModel * envelopes, ExpenseModel * expenses,
                      QWidget * parent);

signals:
    /**
     * Emitted to indicate the currently selected envelope.
     *
     * @param[in] current  Model index of the currently selected envelope
     * @param[in] previous Model index of the previoiusly selected envelope
     */
    void selectEnvelope(const QModelIndex & current, const QModelIndex & previous);

protected slots:
    /**
     * Prompts the user to confirm deletion of the requested envelope.
     *
     * @param index Model index of the envelope to be deleted
     */
    void deleteEnvelope(const QModelIndex & index);

    /**
     * Updates the expense model to display expenses for the selected envelope.
     *
     * @param[in] current  Model index of the currently selected envelope
     * @param[in] previous Model index of the previoiusly selected envelope
     */
    void setExpenseFilter(const QModelIndex & current, const QModelIndex & previous);

    /**
     * Opens the expense details dialog for a new expense.
     */
    void createExpense();

    /**
     * Prompts the user to confirm deletion of the requested budgeted expense.
     *
     * @param index Model index of the budgeted expense to be deleted
     */
    void deleteExpense(const QModelIndex & index);

private:
    /** Envelope model */
    ledger::EnvelopeModel * envelopes_;
    /** Budgeted expense model */
    ExpenseModel * expenses_;

    /** Envelope details dialog */
    ledger::EnvelopeDetailsDialog * envelope_details_;
    /** Expense details dialog */
    ExpenseDetailsDialog * expense_details_;
    /** Envelope tree view */
    ledger::TreeView * tree_;
    /** Budgeted expense table view */
    BudgetTableView * table_;

    /** Currently selected envelope */
    ledger::Envelope envelope_;
};

} // budget namespace
} // ub namespace
