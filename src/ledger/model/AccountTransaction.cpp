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

// Standard include(s)
#include <cstdint>

// Qt include(s)
#include <QtCore>

// UnderBudget include(s)
#include "Account.hpp"
#include "AccountTransaction.hpp"
#include "Transaction.hpp"

namespace ub {
namespace ledger {

//--------------------------------------------------------------------------------------------------
AccountTransaction::AccountTransaction() : AccountTransaction(-1) {}

//--------------------------------------------------------------------------------------------------
AccountTransaction::AccountTransaction(int64_t id)
        : cleared_(false), id_(id), reconciliation_(-1) {}

//--------------------------------------------------------------------------------------------------
Account AccountTransaction::account() const {
    return account_;
}

//--------------------------------------------------------------------------------------------------
Money AccountTransaction::balance() const {
    return balance_;
}

//--------------------------------------------------------------------------------------------------
Money AccountTransaction::amount() const {
    return amount_;
}

//--------------------------------------------------------------------------------------------------
int64_t AccountTransaction::id() const {
    return id_;
}

//--------------------------------------------------------------------------------------------------
bool AccountTransaction::isCleared() const {
    return cleared_;
}

//--------------------------------------------------------------------------------------------------
QString AccountTransaction::memo() const {
    return memo_;
}

//--------------------------------------------------------------------------------------------------
int64_t AccountTransaction::reconciliation() const {
    return reconciliation_;
}

//--------------------------------------------------------------------------------------------------
void AccountTransaction::setAccount(const Account & account) {
    account_ = account;
}

//--------------------------------------------------------------------------------------------------
void AccountTransaction::setAmount(const Money & amount) {
    amount_ = amount;
}

//--------------------------------------------------------------------------------------------------
void AccountTransaction::setBalance(const Money & balance) {
    balance_ = balance;
}

//--------------------------------------------------------------------------------------------------
void AccountTransaction::setCleared(bool cleared) {
    cleared_ = cleared;
}

//--------------------------------------------------------------------------------------------------
void AccountTransaction::setMemo(const QString & memo) {
    memo_ = memo;
}

//--------------------------------------------------------------------------------------------------
void AccountTransaction::setReconciliation(int64_t id) {
    reconciliation_ = id;
}

//--------------------------------------------------------------------------------------------------
void AccountTransaction::setTransaction(const Transaction & transaction) {
    transaction_ = transaction;
}

//--------------------------------------------------------------------------------------------------
Transaction AccountTransaction::transaction() const {
    return transaction_;
}

} // ledger namespace
} // ub namespace
