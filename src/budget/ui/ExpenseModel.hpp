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

// Standard include(s)
#include <memory>
#include <vector>

// Qt include(s)
#include <QAbstractTableModel>
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
};

} // budget namespace
} // ub namespace
