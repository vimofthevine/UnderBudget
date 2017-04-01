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

//--------------------------------------------------------------------------------------------------
void Impact::setType(ImpactType type) {
    type_ = type;
}

//--------------------------------------------------------------------------------------------------
Impact::ImpactType Impact::type() const {
    return type_;
}

} // budget namespace
} // ub namespace
