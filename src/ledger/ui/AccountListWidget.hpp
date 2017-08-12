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

public slots:
    /**
     * Selects the given account.
     *
     * @param account Account to be shown
     */
    void showAccount(const ledger::Account & account);

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
