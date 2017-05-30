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

// Qt include(s)
#include <QAbstractItemModel>
#include <QList>
#include <QModelIndex>
#include <QString>
#include <QVariant>

// UnderBudget include(s)
#include <ledger/model/Account.hpp>

namespace ub {
namespace ledger {

// Forward declaration(s)
class AccountRepository;
class LedgerRepository;
class TransactionRepository;

/**
 * Hierarchical account view model.
 *
 * @ingroup ledger
 */
class AccountModel : public QAbstractItemModel {
    Q_OBJECT

public:
    /** Account name column */
    static const int NAME = 0;
    /** Account balance column */
    static const int BALANCE = 1;

    /**
     * Initializes the account view model.
     */
    AccountModel();

    /**
     * Updates the repositories used by the model.
     *
     * @param repository Ledger repository
     */
    void setRepository(std::shared_ptr<LedgerRepository> repository);

    /**
     * Returns the account at the given model index.
     *
     * @param index Model index of the account to be retrieved
     * @return Account located at the given index
     */
    Account account(const QModelIndex & index) const;

    /**
     * Returns the index to the given account.
     *
     * @param account Account for which to acquire an index
     * @return Index to the account
     */
    QModelIndex index(const Account & account) const;

    /**
     * Creates a new account under the given parent account.
     *
     * @param account Account to be created
     * @param parent Model index of the parent under which to create the account
     * @return @c true if successfully created
     */
    bool create(const Account & account, const QModelIndex & parent);

    /**
     * Updates the account located at the specified index.
     *
     * @param account Account to be updated
     * @param index   Model index of the account to be updated
     * @return @c true if successfully updated
     */
    bool update(const Account & account, const QModelIndex & index);

    /**
     * Removes the account located at the specified index.
     *
     * @param index Model index of the account to be removed
     * @return @c true if successfully removed
     */
    bool remove(const QModelIndex & index);

    int columnCount(const QModelIndex & parent = QModelIndex()) const override;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    QVariant data(const QModelIndex & index, int role) const override;

    int rowCount(const QModelIndex & parent) const override;

    QModelIndex index(int row, int column, const QModelIndex & parent) const override;

    QModelIndex parent(const QModelIndex & child) const override;

signals:
    /** Emitted when an error has occurred. */
    void error(const QString & message) const;

private:
    /** Account repository */
    std::shared_ptr<AccountRepository> accounts_;
    /** Transaction repository */
    std::shared_ptr<TransactionRepository> transactions_;
    /** Header titles */
    QList<QString> headers_;
};

} // ledger namespace
} // ub namespace
