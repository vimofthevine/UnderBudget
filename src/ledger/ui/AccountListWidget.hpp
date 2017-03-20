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

// UnderBudget include(s)
#include <ledger/model/Account.hpp>

// Forward declaration(s)
class QWidget;

namespace ub {
namespace ledger {

// Forward declaration(s)
class AccountDetailsDialog;
class AccountModel;
class AccountTransactionModel;
class Transaction;
class TransactionTableView;
class TreeView;

/**
 * Widget for displaying a list of accounts and associated transactions.
 *
 * @ingroup ledger
 */
class AccountListWidget : public QSplitter {
    Q_OBJECT

public:
    /**
     * Initializes the account list widget.
     *
     * @param[in] model        Account model
     * @param[in] transactions Account transaction model
     * @param[in] parent       Parent widget
     */
    AccountListWidget(AccountModel * model, AccountTransactionModel * transactions,
                      QWidget * parent);

signals:
    /**
     * Emitted to indicate the currently selected account.
     *
     * @param account Currently selected account
     */
    void accountSelected(const Account & account);

    /**
     * Emitted to indicate a request to modify a transaction.
     *
     * @param transaction Transaction to be modified
     */
    void modifyTransaction(const Transaction & transaction);

    /**
     * Emitted to indicate a request to duplicate a transaction.
     *
     * @param transaction Transaction to be duplicated
     */
    void duplicateTransaction(const Transaction & transaction);

    /**
     * Emitted to indicate a request to delete a transaction.
     *
     * @param transaction Transaction to be deleted
     */
    void deleteTransaction(const Transaction & transaction);

private slots:
    /**
     * Prompts the user to confirm deletion of the requested account.
     *
     * @param index Model index of the account to be deleted
     */
    void deleteAccount(const QModelIndex & index);

    /**
     * Updates the account transactions model to display transactions for the selected account.
     *
     * @param[in] current  Model index of the currently selected account
     * @param[in] previous Model index of the previoiusly selected account
     */
    void setTransactionFilter(const QModelIndex & current, const QModelIndex & previous);

    /**
     * Determines which transaction is selected and fires the @c modifyTransaction signal.
     *
     * @param index Model index of the account transaction to be modified
     */
    void modifyAccountTransaction(const QModelIndex & index);

    /**
     * Determines which transaction is selected and fires the @c duplicateTransaction signal.
     *
     * @param index Model index of the account transaction to be duplicated
     */
    void duplicateAccountTransaction(const QModelIndex & index);

    /**
     * Determines which transaction is selected and fires the @c deleteTransaction signal.
     *
     * @param index Model index of the account transaction to be deleted
     */
    void deleteAccountTransaction(const QModelIndex & index);

private:
    /** Account model */
    AccountModel * model_;
    /** Account transaction model */
    AccountTransactionModel * transactions_;

    /** Account details dialog */
    AccountDetailsDialog * details_;
    /** Account tree view */
    TreeView * tree_;
    /** Account transactions list view */
    TransactionTableView * transaction_list_;
};

} // ledger namespace
} // ub namespace
