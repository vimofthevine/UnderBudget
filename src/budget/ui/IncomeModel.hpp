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
#include <budget/model/Income.hpp>
#include <budget/model/IncomeRepository.hpp>
#include <ledger/model/Account.hpp>

namespace ub {
namespace budget {

// Forward declaration(s)
class BudgetRepository;
class IncomeRepository;

/**
 * Budgeted incomes view model.
 *
 * @ingroup budget
 */
class IncomeModel : public QAbstractTableModel {
    Q_OBJECT

public:
    /** Income description column */
    static const int DESCRIPTION = 0;
    /** Income beginning date column */
    static const int BEGINNING_DATE = 1;
    /** Income ending date column */
    static const int ENDING_DATE = 2;
    /** Income recurrence column */
    static const int RECURRENCE = 3;
    /** Income amount column */
    static const int AMOUNT = 4;

    /**
     * Initializes the income view model.
     */
    IncomeModel();

    /**
     * Updates the repositories used by the model.
     *
     * @param repository Budget repository
     */
    void setRepository(std::shared_ptr<BudgetRepository> repository);

    /**
     * Returns the budgeted income at the given income model index.
     *
     * @param index Model index of the budgeted income to be retrieved
     * @return Budgeted income located at the given income model index
     */
    Income income(const QModelIndex & index);

    /**
     * Creates a new budgeted income.
     *
     * @param income Income to be created
     * @return @c true if successfully created
     */
    bool create(const Income & income);

    /**
     * Updates the income located at the specified index.
     *
     * @param income Income to be updated
     * @param index   Model index of the income to be updated
     * @return @c true if successfully updated
     */
    bool update(const Income & income, const QModelIndex & index);

    /**
     * Removes the budgeted income located at the specified index.
     *
     * @param index Model index of the income to be removed
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
     * Limits incomes presented by the model to those belonging to the given account.
     *
     * @param account Account for which to display budgeted incomes
     */
    void filterForAccount(const ledger::Account & account);

    /**
     * Resets the model with updated data from the repository.
     */
    void refresh();

private:
    /** Income repository */
    std::shared_ptr<IncomeRepository> incomes_;
    /** Header titles */
    QList<QString> headers_;
    /** Current account */
    ledger::Account account_;
    /** Cached incomes */
    std::vector<Income> cache_;
};

} // budget namespace
} // ub namespace
