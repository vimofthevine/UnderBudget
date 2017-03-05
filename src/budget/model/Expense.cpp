// Qt include(s)
#include <QDate>

// UnderBudget include(s)
#include <ledger/model/Envelope.hpp>
#include <ledger/model/Money.hpp>
#include "Expense.hpp"
#include "Recurrence.hpp"

namespace ub {
namespace budget {

//--------------------------------------------------------------------------------------------------
Expense::Expense() : Expense(-1) {}

//--------------------------------------------------------------------------------------------------
Expense::Expense(int id) : id_(id) {}

//--------------------------------------------------------------------------------------------------
ledger::Money Expense::amount() const {
    return amount_;
}

//--------------------------------------------------------------------------------------------------
QDate Expense::beginningDate() const {
    return beginning_date_;
}

//--------------------------------------------------------------------------------------------------
QString Expense::description() const {
    return description_;
}

//--------------------------------------------------------------------------------------------------
QDate Expense::endingDate() const {
    return ending_date_;
}

//--------------------------------------------------------------------------------------------------
ledger::Envelope Expense::envelope() const {
    return envelope_;
}

//--------------------------------------------------------------------------------------------------
int Expense::id() const {
    return id_;
}

//--------------------------------------------------------------------------------------------------
Recurrence Expense::recurrence() const {
    return recurrence_;
}

//--------------------------------------------------------------------------------------------------
void Expense::setAmount(const ledger::Money & amount) {
    amount_ = amount;
}

//--------------------------------------------------------------------------------------------------
void Expense::setBeginningDate(const QDate & date) {
    beginning_date_ = date;
}

//--------------------------------------------------------------------------------------------------
void Expense::setDescription(const QString & description) {
    description_ = description;
}

//--------------------------------------------------------------------------------------------------
void Expense::setEndingDate(const QDate & date) {
    ending_date_ = date;
}

//--------------------------------------------------------------------------------------------------
void Expense::setEnvelope(const ledger::Envelope & envelope) {
    envelope_ = envelope;
}

//--------------------------------------------------------------------------------------------------
void Expense::setRecurrence(const Recurrence & recurrence) {
    recurrence_ = recurrence;
}

} // budget namespace
} // ub namespace
