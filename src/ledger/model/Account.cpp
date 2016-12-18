// Qt include(s)
#include <QtCore>

// UnderBudget include(s)
#include "Account.hpp"

namespace ub {
namespace ledger {

//--------------------------------------------------------------------------------------------------
Account::Account() :  id_(0) {}

//--------------------------------------------------------------------------------------------------
Account::Account(int id) : id_(id) {}

//--------------------------------------------------------------------------------------------------
Currency Account::currency() const {
    return currency_;
}

//--------------------------------------------------------------------------------------------------
Money Account::currentBalance() const {
    return current_balance_;
}

//------------------------------------------------------------------------------
int Account::id() const {
    return id_;
}

//------------------------------------------------------------------------------
QString Account::name() const {
    return account_name_;
}

//------------------------------------------------------------------------------
Money Account::reconciledBalance() const {
    return reconciled_balance_;
}

//------------------------------------------------------------------------------
void Account::setCurrency(const Currency & currency) {
    currency_ = currency;
}

//------------------------------------------------------------------------------
void Account::setCurrentBalance(const Money & balance) {
    if (currency_ != balance.currency()) {
        throw std::invalid_argument("");
    }
    current_balance_ = balance;
}

//------------------------------------------------------------------------------
void Account::setName(const QString & name) {
    account_name_ = name;
}

//------------------------------------------------------------------------------
void Account::setReconciledBalance(const Money & balance) {
    if (currency_ != balance.currency()) {
        throw std::invalid_argument("");
    }
    reconciled_balance_ = balance;
}

} // ledger namespace
} // ub namespace
