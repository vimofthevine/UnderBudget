/*
 * Copyright 2016 Kyle Treubig
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

// Qt include(s)
#include <QtCore>

// UnderBudget include(s)
#include "Transaction.hpp"

namespace ub {
namespace ledger {

//--------------------------------------------------------------------------------------------------
Transaction::Transaction() : id_(-1) {}

//--------------------------------------------------------------------------------------------------
Transaction::Transaction(int id) : id_(id) {}

//--------------------------------------------------------------------------------------------------
QDate Transaction::date() const {
    return date_;
}

//--------------------------------------------------------------------------------------------------
int Transaction::id() const {
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
