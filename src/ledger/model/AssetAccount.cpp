// Qt include(s)
#include <QtCore>

// UnderBudget include(s)
#include "AssetAccount.hpp"

namespace ub {
namespace ledger {

//--------------------------------------------------------------------------------------------------
AssetAccount::AssetAccount() :  id_(0) {}

//--------------------------------------------------------------------------------------------------
AssetAccount::AssetAccount(int id) : id_(id) {}

//--------------------------------------------------------------------------------------------------
QString AssetAccount::accountNumber() const {
    return account_number_;
}

//--------------------------------------------------------------------------------------------------
Currency AssetAccount::currency() const {
    return currency_;
}

//--------------------------------------------------------------------------------------------------
Money AssetAccount::currentBalance() const {
    return current_balance_;
}

//------------------------------------------------------------------------------
int AssetAccount::id() const {
    return id_;
}

//------------------------------------------------------------------------------
QString AssetAccount::financialInstitution() const {
    return institution_;
}

//------------------------------------------------------------------------------
QString AssetAccount::name() const {
    return account_name_;
}

//------------------------------------------------------------------------------
Money AssetAccount::reconciledBalance() const {
    return reconciled_balance_;
}

//------------------------------------------------------------------------------
void AssetAccount::setAccountNumber(const QString & number) {
    account_number_ = number;
}

//------------------------------------------------------------------------------
void AssetAccount::setCurrency(const Currency & currency) {
    currency_ = currency;
}

//------------------------------------------------------------------------------
void AssetAccount::setCurrentBalance(const Money & balance) {
    if (currency_ != balance.currency()) {
        throw std::invalid_argument("");
    }
    current_balance_ = balance;
}

//------------------------------------------------------------------------------
void AssetAccount::setFinancialInstitution(const QString & institution) {
    institution_ = institution;
}

//------------------------------------------------------------------------------
void AssetAccount::setName(const QString & name) {
    account_name_ = name;
}

//------------------------------------------------------------------------------
void AssetAccount::setReconciledBalance(const Money & balance) {
    if (currency_ != balance.currency()) {
        throw std::invalid_argument("");
    }
    reconciled_balance_ = balance;
}

} // ledger namespace
} // ub namespace
