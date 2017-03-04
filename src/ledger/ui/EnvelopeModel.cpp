// Standard include(s)
#include <algorithm>
#include <memory>

// Qt include(s)
#include <QtCore>
#include <QtWidgets>

// UnderBudget include(s)
#include <ledger/model/Envelope.hpp>
#include <ledger/model/EnvelopeRepository.hpp>
#include <ledger/model/LedgerRepository.hpp>
#include <ledger/model/TransactionRepository.hpp>
#include "EnvelopeModel.hpp"

namespace ub {
namespace ledger {

//--------------------------------------------------------------------------------------------------
EnvelopeModel::EnvelopeModel() {
    headers_ << tr("Name") << tr("Balance");
}

//--------------------------------------------------------------------------------------------------
void EnvelopeModel::setRepository(std::shared_ptr<LedgerRepository> repository) {
    beginResetModel();
    envelopes_ = repository->envelopes();
    transactions_ = repository->transactions();
    endResetModel();
}

//--------------------------------------------------------------------------------------------------
Envelope EnvelopeModel::envelope(const QModelIndex & index) const {
    if (not envelopes_) {
        return Envelope();
    }

    return index.isValid() ? envelopes_->getEnvelope(index.internalId()) : envelopes_->getRoot();
}

//--------------------------------------------------------------------------------------------------
bool EnvelopeModel::create(const Envelope & envelope, const QModelIndex & parent) {
    if (not envelopes_) {
        return false;
    }

    int rows = rowCount(parent);
    beginInsertRows(parent, rows, rows);
    int id = envelopes_->create(envelope, this->envelope(parent));
    endInsertRows();

    if (id <= 0) {
        emit error(envelopes_->lastError());
    }

    return (id > 0);
}

//--------------------------------------------------------------------------------------------------
bool EnvelopeModel::update(const Envelope & envelope, const QModelIndex & index) {
    if (not envelopes_) {
        return false;
    }

    bool success = envelopes_->update(envelope);
    if (success) {
        emit dataChanged(this->index(index.row(), 0, index.parent()),
                         this->index(index.row(), columnCount(index) - 1, index.parent()));
    } else {
        emit error(envelopes_->lastError());
    }

    return success;
}

//--------------------------------------------------------------------------------------------------
bool EnvelopeModel::remove(const QModelIndex & index) {
    if (not envelopes_) {
        return false;
    }

    beginRemoveRows(index.parent(), index.row(), index.row());
    bool success = envelopes_->remove(this->envelope(index));
    endRemoveRows();

    if (not success) {
        emit error(envelopes_->lastError());
    }

    return success;
}

//--------------------------------------------------------------------------------------------------
int EnvelopeModel::columnCount(const QModelIndex & parent) const {
    return headers_.size();
}

//--------------------------------------------------------------------------------------------------
QVariant EnvelopeModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal and role == Qt::DisplayRole) {
        return headers_.at(section);
    }
    return QVariant();
}

//--------------------------------------------------------------------------------------------------
QVariant EnvelopeModel::data(const QModelIndex & index, int role) const {
    if (not envelopes_ or not index.isValid()) {
        return QVariant();
    }

    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    Envelope envelope = envelopes_->getEnvelope(index.internalId());
    if (envelope.id() == -1) {
        emit error(envelopes_->lastError());
        return QVariant();
    }

    switch (index.column()) {
    case NAME:
        return envelope.name();
    case BALANCE:
        if (envelope.children().size() > 0u) {
            return QVariant();
        } else {
            return transactions_->getBalance(QDate::currentDate(), envelope).toString();
        }
    default:
        return QVariant();
    }
}

//--------------------------------------------------------------------------------------------------
int EnvelopeModel::rowCount(const QModelIndex & parent) const {
    if (not envelopes_ or parent.column() > 0) {
        return 0;
    }

    if (not parent.isValid()) {
        return envelopes_->getRoot().children().size();
    } else {
        return envelopes_->getEnvelope(parent.internalId()).children().size();
    }
}

//--------------------------------------------------------------------------------------------------
QModelIndex EnvelopeModel::index(int row, int column, const QModelIndex & parent) const {
    if (not envelopes_ or not hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    auto children = parent.isValid() ? envelopes_->getEnvelope(parent.internalId()).children()
                                     : envelopes_->getRoot().children();
    if (row < 0 or row >= children.size()) {
        return QModelIndex();
    }
    auto child = children.at(row);

    if (child == -1) {
        return QModelIndex();
    } else {
        return createIndex(row, column, child);
    }
}

//--------------------------------------------------------------------------------------------------
QModelIndex EnvelopeModel::parent(const QModelIndex & child) const {
    if (not envelopes_ or not child.isValid()) {
        return QModelIndex();
    }

    Envelope c_acct = envelopes_->getEnvelope(child.internalId());
    if (c_acct.id() == -1) {
        return QModelIndex();
    }

    Envelope p_acct = envelopes_->getEnvelope(c_acct.parent());
    if (p_acct.id() == -1) {
        return QModelIndex();
    }

    Envelope g_acct = envelopes_->getEnvelope(p_acct.parent());
    if (g_acct.id() == -1) {
        return QModelIndex();
    }

    auto siblings = g_acct.children();
    auto iter = std::find(siblings.begin(), siblings.end(), p_acct.id());
    return createIndex(std::distance(siblings.begin(), iter), 0, p_acct.id());
}

} // ledger namespace
} // ub namespace
