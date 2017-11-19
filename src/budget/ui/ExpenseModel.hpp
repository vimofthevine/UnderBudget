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

// Standard include(s)
#include <memory>
#include <vector>

// Qt include(s)
#include <QAbstractTableModel>
#include <QDate>
#include <QList>
#include <QModelIndex>
#include <QString>
#include <QVariant>

// UnderBudget include(s)
#include <budget/model/Expense.hpp>
#include <budget/model/ExpenseRepository.hpp>
#include <ledger/model/Envelope.hpp>

namespace ub {
namespace budget {

// Forward declaration(s)
class BudgetRepository;
class ExpenseRepository;

/**
 * Budgeted expenses view model.
 *
 * @ingroup budget
 */
class ExpenseModel : public QAbstractTableModel {
    Q_OBJECT

public:
    /** Expense description column */
    static const int DESCRIPTION = 0;
    /** Expense beginning date column */
    static const int BEGINNING_DATE = 1;
    /** Expense ending date column */
    static const int ENDING_DATE = 2;
    /** Expense recurrence column */
    static const int RECURRENCE = 3;
    /** Expense amount column */
    static const int AMOUNT = 4;

    /**
     * Initializes the expense view model.
     */
    ExpenseModel();

    /**
     * Updates the repositories used by the model.
     *
     * @param repository Budget repository
     */
    void setRepository(std::shared_ptr<BudgetRepository> repository);

    /**
     * Returns the budgeted expense at the given expense model index.
     *
     * @param index Model index of the budgeted expense to be retrieved
     * @return Budgeted expense located at the given expense model index
     */
    Expense expense(const QModelIndex & index);

    /**
     * Creates a new budgeted expense.
     *
     * @param expense Expense to be created
     * @return @c true if successfully created
     */
    bool create(const Expense & expense);

    /**
     * Updates the expense located at the specified index.
     *
     * @param expense Expense to be updated
     * @param index   Model index of the expense to be updated
     * @return @c true if successfully updated
     */
    bool update(const Expense & expense, const QModelIndex & index);

    /**
     * Removes the budgeted expense located at the specified index.
     *
     * @param index Model index of the expense to be removed
     * @return @c true if successfully removed
     */
    bool remove(const QModelIndex & index);

    int columnCount(const QModelIndex & parent) const override;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    int rowCount(const QModelIndex & parent) const override;

    QVariant data(const QModelIndex & index, int role) const override;

signals:
    /** Emitted when an error has occurred. */
    void error(const QString & message) const;

public slots:
    /**
     * Limits expenses presented by the model to those belonging to the given envelope.
     *
     * @param envelope Envelope for which to display budgeted expenses
     */
    void filterForEnvelope(const ledger::Envelope & envelope);

    /**
     * Limits expenses presented by the model to those occuring within the given date range.
     *
     * @param begin Beginning date
     * @param end   Ending date
     */
    void filterForDates(const QDate & begin, const QDate & end);

    /**
     * Resets the model with updated data from the repository.
     */
    void refresh();

private:
    /** Expense repository */
    std::shared_ptr<ExpenseRepository> expenses_;
    /** Header titles */
    QList<QString> headers_;
    /** Current envelope */
    ledger::Envelope envelope_;
    /** Cached expenses */
    std::vector<Expense> cache_;
    /** Beginning date for which to filter displayed expenses */
    QDate beginning_date_;
    /** Ending date for which to filter displayed expenses */
    QDate ending_date_;

    /**
     * Rebuilds the cache of expenses for display
     */
    void rebuildCache();
};

} // budget namespace
} // ub namespace
