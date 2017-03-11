/*
 * Copyright 2017 Kyle Treubig
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
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
#include <ledger/model/Account.hpp>
#include <ledger/model/AccountTransaction.hpp>

// Forward declaration(s)
class QObject;

namespace ub {
namespace ledger {

// Forward declaration(s)
class JournalEntry;

/**
 * Account splits view model.
 *
 * This view model is intended to be used for displaying the account transactions associated
 * with a journal entry, rather than the @c AccountTransactionModel that is used to view
 * transactions from the repository.
 *
 * @ingroup ledger
 */
class AccountSplitModel : public QAbstractTableModel {
    Q_OBJECT

public:
    /**
     * Initializes the account split view model.
     *
     * @param parent Parent object
     */
    AccountSplitModel(QObject * parent);

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
