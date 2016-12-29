// Qt include(s)
#include <QtCore>

// UnderBudget include(s)
#include "AccountTransaction.hpp"

namespace ub {
namespace ledger {

//--------------------------------------------------------------------------------------------------
AccountTransaction::AccountTransaction() : AccountTransaction(-1) {}

//--------------------------------------------------------------------------------------------------
AccountTransaction::AccountTransaction(int id)
    : account_(-1), amount_(0), cleared_(false), id_(id), reconciliation_(-1), transaction_(-1) {}

//--------------------------------------------------------------------------------------------------
int AccountTransaction::account() const {
    return account_;
}

//--------------------------------------------------------------------------------------------------
int AccountTransaction::amount() const {
    return amount_;
}

//--------------------------------------------------------------------------------------------------
int AccountTransaction::id() const {
    return id_;
}

//--------------------------------------------------------------------------------------------------
bool AccountTransaction::isCleared() const {
    return cleared_;
}

//--------------------------------------------------------------------------------------------------
QString AccountTransaction::memo() const {
    return memo_;
}

//--------------------------------------------------------------------------------------------------
int AccountTransaction::reconciliation() const {
    return reconciliation_;
}

//--------------------------------------------------------------------------------------------------
void AccountTransaction::setAccount(int id) {
    account_ = id;
}

//--------------------------------------------------------------------------------------------------
void AccountTransaction::setAmount(int amount) {
    amount_ = amount;
}

//--------------------------------------------------------------------------------------------------
void AccountTransaction::setCleared(bool cleared) {
    cleared_ = cleared;
}

//--------------------------------------------------------------------------------------------------
void AccountTransaction::setMemo(const QString & memo) {
    memo_ = memo;
}

//--------------------------------------------------------------------------------------------------
void AccountTransaction::setReconciliation(int id) {
    reconciliation_ = id;
}

//--------------------------------------------------------------------------------------------------
void AccountTransaction::setTransaction(int id) {
    transaction_ = id;
}

//--------------------------------------------------------------------------------------------------
int AccountTransaction::transaction() const {
    return transaction_;
}

} // ledger namespace
} // ub namespace
