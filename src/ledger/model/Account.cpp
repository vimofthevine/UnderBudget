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

// Standard include(s)
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
Account::Account(int id) : archived_(false), id_(id), parent_(-1) {}

//--------------------------------------------------------------------------------------------------
bool Account::archived() const {
    return archived_;
}

//--------------------------------------------------------------------------------------------------
std::vector<int> Account::children() const {
    return children_;
}

//--------------------------------------------------------------------------------------------------
Currency Account::currency() const {
    return currency_;
}

//------------------------------------------------------------------------------
int Account::id() const {
    return id_;
}

//------------------------------------------------------------------------------
QString Account::name() const {
    return account_name_;
}

//------------------------------------------------------------------------------
int Account::parent() const {
    return parent_;
}

//------------------------------------------------------------------------------
void Account::setArchived(bool archived) {
    archived_ = archived;
}

//------------------------------------------------------------------------------
void Account::setChildren(const std::vector<int> & children) {
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
void Account::setParent(int id) {
    parent_ = id;
}

} // ledger namespace
} // ub namespace
