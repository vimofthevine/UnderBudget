// Qt include(s)
#include <QtCore>

// UnderBudget include(s)
#include "Account.hpp"
#include "AccountTransaction.hpp"
#include "Transaction.hpp"

namespace ub {
namespace ledger {

//--------------------------------------------------------------------------------------------------
AccountTransaction::AccountTransaction() : AccountTransaction(-1) {}

//--------------------------------------------------------------------------------------------------
AccountTransaction::AccountTransaction(int id)
    : cleared_(false), id_(id), reconciliation_(-1) {}

//--------------------------------------------------------------------------------------------------
Account AccountTransaction::account() const {
    return account_;
}

//--------------------------------------------------------------------------------------------------
Money AccountTransaction::balance() const {
    return balance_;
}

//--------------------------------------------------------------------------------------------------
Money AccountTransaction::amount() const {
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
void AccountTransaction::setAccount(const Account & account) {
    account_ = account;
}

//--------------------------------------------------------------------------------------------------
void AccountTransaction::setAmount(const Money & amount) {
    amount_ = amount;
}

//--------------------------------------------------------------------------------------------------
void AccountTransaction::setBalance(const Money & balance) {
    balance_ = balance;
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
void AccountTransaction::setTransaction(const Transaction & transaction) {
    transaction_ = transaction;
}

//--------------------------------------------------------------------------------------------------
Transaction AccountTransaction::transaction() const {
    return transaction_;
}

} // ledger namespace
} // ub namespace
