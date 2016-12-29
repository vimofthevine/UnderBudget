// Qt include(s)
#include <QtCore>

// UnderBudget include(s)
#include "Envelope.hpp"
#include "EnvelopeTransaction.hpp"

namespace ub {
namespace ledger {

//--------------------------------------------------------------------------------------------------
EnvelopeTransaction::EnvelopeTransaction() : EnvelopeTransaction(-1) {}

//--------------------------------------------------------------------------------------------------
EnvelopeTransaction::EnvelopeTransaction(int id)
    : cleared_(false), id_(id), reconciliation_(-1), transaction_(-1) {}

//--------------------------------------------------------------------------------------------------
Money EnvelopeTransaction::amount() const {
    return amount_;
}

//--------------------------------------------------------------------------------------------------
Envelope EnvelopeTransaction::envelope() const {
    return envelope_;
}

//--------------------------------------------------------------------------------------------------
int EnvelopeTransaction::id() const {
    return id_;
}

//--------------------------------------------------------------------------------------------------
bool EnvelopeTransaction::isCleared() const {
    return cleared_;
}

//--------------------------------------------------------------------------------------------------
QString EnvelopeTransaction::memo() const {
    return memo_;
}

//--------------------------------------------------------------------------------------------------
int EnvelopeTransaction::reconciliation() const {
    return reconciliation_;
}

//--------------------------------------------------------------------------------------------------
void EnvelopeTransaction::setAmount(const Money & amount) {
    amount_ = amount;
}

//--------------------------------------------------------------------------------------------------
void EnvelopeTransaction::setCleared(bool cleared) {
    cleared_ = cleared;
}

//--------------------------------------------------------------------------------------------------
void EnvelopeTransaction::setEnvelope(const Envelope & envelope) {
    envelope_ = envelope;
}

//--------------------------------------------------------------------------------------------------
void EnvelopeTransaction::setMemo(const QString & memo) {
    memo_ = memo;
}

//--------------------------------------------------------------------------------------------------
void EnvelopeTransaction::setReconciliation(int id) {
    reconciliation_ = id;
}

//--------------------------------------------------------------------------------------------------
void EnvelopeTransaction::setTransaction(int id) {
    transaction_ = id;
}

//--------------------------------------------------------------------------------------------------
int EnvelopeTransaction::transaction() const {
    return transaction_;
}

} // ledger namespace
} // ub namespace
