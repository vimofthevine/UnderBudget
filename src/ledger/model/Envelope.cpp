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
#include "Envelope.hpp"

namespace ub {
namespace ledger {

//------------------------------------------------------------------------------
Envelope::Envelope() : Envelope(-1) {}

//------------------------------------------------------------------------------
Envelope::Envelope(int id) : archived_(false), id_(id), parent_(-1) {}

//------------------------------------------------------------------------------
bool Envelope::archived() const {
    return archived_;
}

//------------------------------------------------------------------------------
std::vector<int> Envelope::children() const {
    return children_;
}

//------------------------------------------------------------------------------
Currency Envelope::currency() const {
    return currency_;
}

//------------------------------------------------------------------------------
int Envelope::id() const {
    return id_;
}

//------------------------------------------------------------------------------
QString Envelope::name() const {
    return name_;
}

//------------------------------------------------------------------------------
int Envelope::parent() const {
    return parent_;
}

//------------------------------------------------------------------------------
void Envelope::setArchived(bool archived) {
    archived_ = archived;
}

//------------------------------------------------------------------------------
void Envelope::setChildren(const std::vector<int> & children) {
    children_ = children;
}

//------------------------------------------------------------------------------
void Envelope::setCurrency(const Currency & currency) {
    currency_ = currency;
}

//------------------------------------------------------------------------------
void Envelope::setName(const QString & name) {
    name_ = name;
}

//------------------------------------------------------------------------------
void Envelope::setParent(int id) {
    parent_ = id;
}

} // ledger namespace
} // ub namespace
