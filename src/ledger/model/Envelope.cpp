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
