// Qt include(s)
#include <QtCore>

// UnderBudget include(s)
#include "Envelope.hpp"
#include "EnvelopeTransaction.hpp"
#include "Transaction.hpp"

namespace ub {
namespace ledger {

//--------------------------------------------------------------------------------------------------
EnvelopeTransaction::EnvelopeTransaction() : id_(-1) {}

//--------------------------------------------------------------------------------------------------
EnvelopeTransaction::EnvelopeTransaction(int id) : id_(id) {}

//--------------------------------------------------------------------------------------------------
Money EnvelopeTransaction::amount() const {
    return amount_;
}

//--------------------------------------------------------------------------------------------------
Money EnvelopeTransaction::balance() const {
    return balance_;
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
QString EnvelopeTransaction::memo() const {
    return memo_;
}

//--------------------------------------------------------------------------------------------------
void EnvelopeTransaction::setAmount(const Money & amount) {
    amount_ = amount;
}

//--------------------------------------------------------------------------------------------------
void EnvelopeTransaction::setBalance(const Money & balance) {
    balance_ = balance;
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
void EnvelopeTransaction::setTransaction(const Transaction & transaction) {
    transaction_ = transaction;
}

//--------------------------------------------------------------------------------------------------
Transaction EnvelopeTransaction::transaction() const {
    return transaction_;
}

} // ledger namespace
} // ub namespace
