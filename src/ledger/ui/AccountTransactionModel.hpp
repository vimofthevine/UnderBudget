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
#include <QList>
#include <QModelIndex>
#include <QString>
#include <QVariant>

// UnderBudget include(s)
#include <ledger/model/Account.hpp>
#include <ledger/model/AccountTransaction.hpp>
#include <ledger/model/Transaction.hpp>

namespace ub {
namespace ledger {

// Forward declaration(s)
class LedgerRepository;
class TransactionRepository;

/**
 * Account transactions view model.
 *
 * @ingroup ledger
 */
class AccountTransactionModel : public QAbstractTableModel {
    Q_OBJECT

public:
    /** Transaction date column */
    static const int DATE = 0;
    /** Transaction payee column */
    static const int PAYEE = 1;
    /** Transaction memo column */
    static const int MEMO = 2;
    /** Transaction amount column */
    static const int AMOUNT = 3;
    /** Account balance column */
    static const int BALANCE = 4;

    /**
     * Initializes the account transaction view model.
     */
    AccountTransactionModel();

    /**
     * Updates the repositories used by the model.
     *
     * @param repository Ledger repository
     */
    void setRepository(std::shared_ptr<LedgerRepository> repository);

    /**
     * Returns the transaction at the given account transaction model index.
     *
     * @param index Model index of the account transaction to be retrieved.
     * @return Transaction located at the given account transaction index
     */
    Transaction transaction(const QModelIndex & index);

    int columnCount(const QModelIndex & parent) const override;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    int rowCount(const QModelIndex & parent) const override;

    QVariant data(const QModelIndex & index, int role) const override;

    Qt::ItemFlags flags(const QModelIndex & index) const override;

signals:
    /** Emitted when an error has occurred. */
    void error(const QString & message) const;

public slots:
    /**
     * Limits transactions presented by the model to those belonging to the given account.
     *
     * @param account Account for which to display transactions
     */
    void filterForAccount(const Account & account);

    /**
     * Resets the model with updated data from the repository.
     */
    void refresh();

private:
    /** Transaction repository */
    std::shared_ptr<TransactionRepository> transactions_;
    /** Header titles */
    QList<QString> headers_;
    /** Current account */
    Account account_;
    /** Cached transactions */
    std::vector<AccountTransaction> cache_;
};

} // ledger namespace
} // ub namespace
