// // Standard include(s)
#include <vector>

// Qt include(s)
#include <QtCore>

// UnderBudget include(s)
#include "Account.hpp"

namespace ub {
namespace ledger {

//--------------------------------------------------------------------------------------------------
Account::Account() :  id_(0), parent_(0) {}

//--------------------------------------------------------------------------------------------------
Account::Account(int id) : id_(id), parent_(0) {}

//--------------------------------------------------------------------------------------------------
std::vector<int> Account::children() const {
    return children_;
}

//--------------------------------------------------------------------------------------------------
Currency Account::currency() const {
    return currency_;
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
int Account::parent() const {
    return parent_;
}

//------------------------------------------------------------------------------
void Account::setChildren(const std::vector<int> & children) {
    children_ = children;
}

//------------------------------------------------------------------------------
void Account::setCurrency(const Currency & currency) {
    currency_ = currency;
}

//------------------------------------------------------------------------------
void Account::setName(const QString & name) {
    account_name_ = name;
}

} // ledger namespace
} // ub namespace
