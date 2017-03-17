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

// Qt include(s)
#include <QDate>
#include <QString>

// UnderBudget include(s)
#include <ledger/model/Money.hpp>
#include "Impact.hpp"

namespace ub {
namespace budget {

//--------------------------------------------------------------------------------------------------
ledger::Money Impact::amount() const {
    return amount_;
}

//--------------------------------------------------------------------------------------------------
QString Impact::category() const {
    return category_;
}

//--------------------------------------------------------------------------------------------------
QDate Impact::date() const {
    return date_;
}

//--------------------------------------------------------------------------------------------------
QString Impact::description() const {
    return description_;
}

//--------------------------------------------------------------------------------------------------
void Impact::setAmount(const ledger::Money & amount) {
    amount_ = amount;
}

//--------------------------------------------------------------------------------------------------
void Impact::setCategory(const QString & category) {
    category_ = category;
}

//--------------------------------------------------------------------------------------------------
void Impact::setDate(const QDate & date) {
    date_ = date;
}

//--------------------------------------------------------------------------------------------------
void Impact::setDescription(const QString & description) {
    description_ = description;
}

} // budget namespace
} // ub namespace
