/*
 * UnderBudget
 * Copyright 2017 Kyle Treubig
 *
 * UnderBudget is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * UnderBudget is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with UnderBudget.  If not, see <http://www.gnu.org/licenses/>.
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
