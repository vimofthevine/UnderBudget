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
#include <vector>

// Qt include(s)
#include <QtCore>

// UnderBudget include(s)
#include "Account.hpp"

namespace ub {
namespace ledger {

//--------------------------------------------------------------------------------------------------
Account::Account() : Account(-1) {}

//--------------------------------------------------------------------------------------------------
Account::Account(int64_t id) : archived_(false), id_(id), parent_(-1) {}

//--------------------------------------------------------------------------------------------------
bool Account::archived() const {
    return archived_;
}

//--------------------------------------------------------------------------------------------------
std::vector<int64_t> Account::children() const {
    return children_;
}

//--------------------------------------------------------------------------------------------------
Currency Account::currency() const {
    return currency_;
}

//------------------------------------------------------------------------------
int64_t Account::id() const {
    return id_;
}

//------------------------------------------------------------------------------
QString Account::name() const {
    return account_name_;
}

//------------------------------------------------------------------------------
int64_t Account::parent() const {
    return parent_;
}

//------------------------------------------------------------------------------
void Account::setArchived(bool archived) {
    archived_ = archived;
}

//------------------------------------------------------------------------------
void Account::setChildren(const std::vector<int64_t> & children) {
    children_ = children;
}

//------------------------------------------------------------------------------
void Account::setCurrency(const Currency & currency) {
    currency_ = currency;
}

//------------------------------------------------------------------------------
void Account::setName(const QString & name) {
    account_name_ = name;
}

//------------------------------------------------------------------------------
void Account::setParent(int64_t id) {
    parent_ = id;
}

} // ledger namespace
} // ub namespace
