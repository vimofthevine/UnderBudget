// Qt include(s)
#include <QDate>

// UnderBudget include(s)
#include "Account.hpp"
#include "Money.hpp"
#include "Reconciliation.hpp"

namespace ub {
namespace ledger {

//--------------------------------------------------------------------------------------------------
Reconciliation::Reconciliation() : Reconciliation(-1) {}

//--------------------------------------------------------------------------------------------------
Reconciliation::Reconciliation(int id) : id_(id) {}

//--------------------------------------------------------------------------------------------------
Account Reconciliation::account() const {
    return account_;
}

//--------------------------------------------------------------------------------------------------
Money Reconciliation::beginningBalance() const {
    return beginning_balance_;
}

//--------------------------------------------------------------------------------------------------
QDate Reconciliation::beginningDate() const {
    return beginning_date_;
}

//--------------------------------------------------------------------------------------------------
Money Reconciliation::endingBalance() const {
    return ending_balance_;
}

//--------------------------------------------------------------------------------------------------
QDate Reconciliation::endingDate() const {
    return ending_date_;
}

//--------------------------------------------------------------------------------------------------
int Reconciliation::id() const {
    return id_;
}

//--------------------------------------------------------------------------------------------------
void Reconciliation::setAccount(const Account &account) {
    account_ = account;
}

//--------------------------------------------------------------------------------------------------
void Reconciliation::setBeginningBalance(const Money & balance) {
    beginning_balance_ = balance;
}

//--------------------------------------------------------------------------------------------------
void Reconciliation::setBeginningDate(const QDate & date) {
    beginning_date_ = date;
}

//--------------------------------------------------------------------------------------------------
void Reconciliation::setEndingBalance(const Money & balance) {
    ending_balance_ = balance;
}

//--------------------------------------------------------------------------------------------------
void Reconciliation::setEndingDate(const QDate & date) {
    ending_date_ = date;
}

} // ledger namespace
} // ub namespace
