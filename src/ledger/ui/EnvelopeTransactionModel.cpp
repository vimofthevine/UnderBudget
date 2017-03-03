// Qt include(s)
#include <QtWidgets>

// UnderBudget include(s)
#include <ledger/model/Envelope.hpp>
#include <ledger/model/LedgerRepository.hpp>
#include <ledger/model/TransactionRepository.hpp>
#include "EnvelopeTransactionModel.hpp"

namespace ub {
namespace ledger {

//--------------------------------------------------------------------------------------------------
EnvelopeTransactionModel::EnvelopeTransactionModel() {
    headers_ << tr("Date") << tr("Payee") << tr("Memo") << tr("Amount") << tr("Balance");
}

//--------------------------------------------------------------------------------------------------
void EnvelopeTransactionModel::setRepository(std::shared_ptr<LedgerRepository> repository) {
    beginResetModel();
    envelope_ = Envelope();
    transactions_ = repository->transactions();
    cache_.clear();
    endResetModel();
}

//--------------------------------------------------------------------------------------------------
void EnvelopeTransactionModel::filterForEnvelope(const Envelope &envelope) {
    beginResetModel();
    envelope_ = envelope;
    if (transactions_) {
        cache_ = transactions_->getTransactions(envelope);
    }
    endResetModel();
}

//--------------------------------------------------------------------------------------------------
int EnvelopeTransactionModel::columnCount(const QModelIndex &parent) const {
    return headers_.size();
}

//--------------------------------------------------------------------------------------------------
QVariant EnvelopeTransactionModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if ((orientation == Qt::Horizontal) and (role == Qt::DisplayRole)) {
        return headers_.at(section);
    }
    return QVariant();
}

//--------------------------------------------------------------------------------------------------
QVariant EnvelopeTransactionModel::data(const QModelIndex &index, int role) const {
    if (not index.isValid()) {
        return QVariant();
    }

    if ((role != Qt::DisplayRole) and (role != Qt::EditRole)){
        return QVariant();
    }

    int row = index.row();
    if ((row < 0) or (row >= cache_.size())) {
        return QVariant();
    }

    auto transaction = cache_.at(row);
    switch (index.column()) {
    case DATE:
        return transaction.transaction().date();
    case PAYEE:
        return transaction.transaction().payee();
    case MEMO:
        return transaction.memo();
    case AMOUNT:
        return (role == Qt::DisplayRole) ? transaction.amount().toString()
                                         : QVariant::fromValue(transaction.amount());
    case BALANCE:
        return (role == Qt::DisplayRole) ? transaction.balance().toString()
                                         : QVariant::fromValue(transaction.balance());
    default:
        return QVariant();
    }
}

//--------------------------------------------------------------------------------------------------
int EnvelopeTransactionModel::rowCount(const QModelIndex &parent) const {
    if (parent.column() > 0) {
        return 0;
    }

    return cache_.size();
}

//--------------------------------------------------------------------------------------------------
Qt::ItemFlags EnvelopeTransactionModel::flags(const QModelIndex &index) const {
    return QAbstractTableModel::flags(index);
}

} // ledger namespace
} // ub namespace
