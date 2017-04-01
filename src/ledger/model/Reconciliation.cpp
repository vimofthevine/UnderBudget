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
#include <QDate>

// UnderBudget include(s)
#include "Account.hpp"
#include "Money.hpp"
#include "Reconciliation.hpp"

namespace ub {
namespace ledger {

//--------------------------------------------------------------------------------------------------
Reconciliation::Reconciliation() : Reconciliation(-1) {}

//--------------------------------------------------------------------------------------------------
Reconciliation::Reconciliation(int64_t id) : id_(id) {}

//--------------------------------------------------------------------------------------------------
Account Reconciliation::account() const {
    return account_;
}

//--------------------------------------------------------------------------------------------------
Money Reconciliation::beginningBalance() const {
    return beginning_balance_;
}

//--------------------------------------------------------------------------------------------------
QDate Reconciliation::beginningDate() const {
    return beginning_date_;
}

//--------------------------------------------------------------------------------------------------
Money Reconciliation::endingBalance() const {
    return ending_balance_;
}

//--------------------------------------------------------------------------------------------------
QDate Reconciliation::endingDate() const {
    return ending_date_;
}

//--------------------------------------------------------------------------------------------------
int64_t Reconciliation::id() const {
    return id_;
}

//--------------------------------------------------------------------------------------------------
void Reconciliation::setAccount(const Account & account) {
    account_ = account;
}

//--------------------------------------------------------------------------------------------------
void Reconciliation::setBeginningBalance(const Money & balance) {
    beginning_balance_ = balance;
}

//--------------------------------------------------------------------------------------------------
void Reconciliation::setBeginningDate(const QDate & date) {
    beginning_date_ = date;
}

//--------------------------------------------------------------------------------------------------
void Reconciliation::setEndingBalance(const Money & balance) {
    ending_balance_ = balance;
}

//--------------------------------------------------------------------------------------------------
void Reconciliation::setEndingDate(const QDate & date) {
    ending_date_ = date;
}

} // ledger namespace
} // ub namespace
