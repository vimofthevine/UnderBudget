// Qt include(s)
#include <QtCore>

// UnderBudget include(s)
#include "Transaction.hpp"

namespace ub {
namespace ledger {

//--------------------------------------------------------------------------------------------------
Transaction::Transaction() : id_(-1) {}

//--------------------------------------------------------------------------------------------------
Transaction::Transaction(int id) : id_(id) {}

//--------------------------------------------------------------------------------------------------
QDate Transaction::date() const {
    return date_;
}

//--------------------------------------------------------------------------------------------------
int Transaction::id() const {
    return id_;
}

//--------------------------------------------------------------------------------------------------
QString Transaction::payee() const {
    return payee_;
}

//--------------------------------------------------------------------------------------------------
void Transaction::setDate(const QDate & date) {
    date_ = date;
}

//--------------------------------------------------------------------------------------------------
void Transaction::setPayee(const QString & payee) {
    payee_ = payee;
}

} // ledger namespace
} // ub namespace
