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
#include "Transaction.hpp"

namespace ub {
namespace ledger {

//--------------------------------------------------------------------------------------------------
Transaction::Transaction() : id_(-1) {}

//--------------------------------------------------------------------------------------------------
Transaction::Transaction(int64_t id) : id_(id) {}

//--------------------------------------------------------------------------------------------------
QDate Transaction::date() const {
    return date_;
}

//--------------------------------------------------------------------------------------------------
int64_t Transaction::id() const {
    return id_;
}

//--------------------------------------------------------------------------------------------------
QString Transaction::payee() const {
    return payee_;
}

//--------------------------------------------------------------------------------------------------
void Transaction::setDate(const QDate & date) {
    date_ = date;
}

//--------------------------------------------------------------------------------------------------
void Transaction::setPayee(const QString & payee) {
    payee_ = payee;
}

} // ledger namespace
} // ub namespace
