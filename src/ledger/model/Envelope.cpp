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
#include "Envelope.hpp"

namespace ub {
namespace ledger {

//------------------------------------------------------------------------------
Envelope::Envelope() : Envelope(-1) {}

//------------------------------------------------------------------------------
Envelope::Envelope(int64_t id) : archived_(false), id_(id), parent_(-1) {}

//------------------------------------------------------------------------------
bool Envelope::archived() const {
    return archived_;
}

//------------------------------------------------------------------------------
std::vector<int64_t> Envelope::children() const {
    return children_;
}

//------------------------------------------------------------------------------
Currency Envelope::currency() const {
    return currency_;
}

//------------------------------------------------------------------------------
QString Envelope::externalId() const {
    return ext_id_;
}

//------------------------------------------------------------------------------
int64_t Envelope::id() const {
    return id_;
}

//------------------------------------------------------------------------------
QString Envelope::name() const {
    return name_;
}

//------------------------------------------------------------------------------
int64_t Envelope::parent() const {
    return parent_;
}

//------------------------------------------------------------------------------
void Envelope::setArchived(bool archived) {
    archived_ = archived;
}

//------------------------------------------------------------------------------
void Envelope::setChildren(const std::vector<int64_t> & children) {
    children_ = children;
}

//------------------------------------------------------------------------------
void Envelope::setCurrency(const Currency & currency) {
    currency_ = currency;
}

//------------------------------------------------------------------------------
void Envelope::setExternalId(const QString & id) {
    ext_id_ = id;
}

//------------------------------------------------------------------------------
void Envelope::setName(const QString & name) {
    name_ = name;
}

//------------------------------------------------------------------------------
void Envelope::setParent(int64_t id) {
    parent_ = id;
}

} // namespace ledger
} // namespace ub
