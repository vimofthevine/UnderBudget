// Qt include(s)
#include <QtCore>

// UnderBudget include(s)
#include "Envelope.hpp"

namespace ub {
namespace ledger {

//------------------------------------------------------------------------------
Envelope::Envelope() : id_(0) {}

//------------------------------------------------------------------------------
Envelope::Envelope(int id) : id_(id) {}

//------------------------------------------------------------------------------
QString Envelope::category() const {
    return category_;
}

//------------------------------------------------------------------------------
Currency Envelope::currency() const {
    return currency_;
}

//------------------------------------------------------------------------------
Money Envelope::currentBalance() const {
    return current_balance_;
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
void Envelope::setCategory(const QString & category) {
    category_ = category;
}

//------------------------------------------------------------------------------
void Envelope::setCurrency(const Currency & currency) {
    currency_ = currency;
}

//------------------------------------------------------------------------------
void Envelope::setCurrentBalance(const Money & balance) {
    if (currency_ != balance.currency()) {
        throw std::invalid_argument("");
    }
    current_balance_ = balance;
}

//------------------------------------------------------------------------------
void Envelope::setName(const QString & name) {
    name_ = name;
}

} // ledger namespace
} // ub namespace
