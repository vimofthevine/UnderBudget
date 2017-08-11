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
#include <cstdint>
#include <map>
#include <memory>
#include <vector>

// Qt include(s)
#include <QAbstractItemModel>

// UnderBudget include(s)
#include <budget/model/Impact.hpp>
#include <ledger/model/Account.hpp>
#include <ledger/model/AccountTransaction.hpp>
#include <ledger/model/Money.hpp>
#include <ledger/ui/AccountModel.hpp>

namespace ub {

namespace ledger {
class AccountRepository;
class LedgerRepository;
}

namespace report {

class ProjectedIncomeModel : public ledger::AccountModel {
    Q_OBJECT

public:
    /** Projected impact column */
    static const int PROJECTED = 1;
    /** Actual expense column */
    static const int ACTUAL = 2;
    /** Difference column */
    static const int DIFFERENCE = 3;

    /**
     * Initializes the projected income view model.
     */
    ProjectedIncomeModel();

    /**
     * Retrieves the account represented by the given model idnex.
     *
     * @param index Model index
     * @return Account located at the given index
     */
    ledger::Account account(const QModelIndex & index) const;

    /**
     * Resets the model for new data.
     */
    void reset();

    /**
     * Updates the projected impacts to be represented by the model.
     *
     * @param impacts Projected impacts list
     */
    void setProjectedImpacts(const std::vector<budget::Impact> & impacts);

    /**
     * Updates the actual incomes to be represented by the model.
     *
     * @param incomes Actual incomes list
     */
    void setActualIncomes(const std::vector<ledger::AccountTransaction> & income);

    /**
     * Updates the repositories used by the model.
     *
     * @param repository Ledger repository
     */
    void setRepository(std::shared_ptr<ledger::LedgerRepository> repository);

    int columnCount(const QModelIndex & parent = QModelIndex()) const override;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    QVariant data(const QModelIndex & index, int role) const override;

private:
    /** Header titles */
    QList<QString> headers_;
    /** Account repository */
    std::shared_ptr<ledger::AccountRepository> accounts_;
    /** Projected impact per account */
    std::map<int64_t, ledger::Money> projected_;
    /** Actual impact per account */
    std::map<int64_t, ledger::Money> actual_;
    /** Differences per account */
    std::map<int64_t, ledger::Money> difference_;
};

} // report namespace
} // ub namespace
