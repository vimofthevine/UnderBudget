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
