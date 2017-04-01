/*
 * UnderBudget
 * Copyright 2017 Kyle Treubig
 *
 * UnderBudget is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * UnderBudget is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with UnderBudget.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

// Standard include(s)
#include <memory>

// Qt include(s)
#include <QAbstractTableModel>
#include <QList>
#include <QModelIndex>
#include <QString>
#include <QVariant>

// UnderBudget include(s)
#include <ledger/model/Envelope.hpp>
#include <ledger/model/EnvelopeTransaction.hpp>

// Forward declaration(s)
class QObject;

namespace ub {
namespace ledger {

// Forward declaration(s)
class JournalEntry;

/**
 * Envelope splits view model.
 *
 * This view model is intended to be used for displaying the envelope transactions associated
 * with a journal entry, rather than the @c EnvelopeTransactionModel that is used to view
 * transactions from the repository.
 *
 * @ingroup ledger
 */
class EnvelopeSplitModel : public QAbstractTableModel {
    Q_OBJECT

public:
    /**
     * Initializes the envelope split view model.
     *
     * @param parent Parent object
     */
    EnvelopeSplitModel(QObject * parent);

    /**
     * Updates the journal entry to be used by the model.
     *
     * @param entry Journal entry
     */
    void setJournalEntry(std::shared_ptr<JournalEntry> entry);

    int columnCount(const QModelIndex & parent) const override;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    int rowCount(const QModelIndex & parent) const override;

    QVariant data(const QModelIndex & index, int role) const override;

private:
    /** Journal entry */
    std::shared_ptr<JournalEntry> entry_;
    /** Header titles */
    QList<QString> headers_;
};

} // ledger namespace
} // ub namespace
