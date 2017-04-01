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

// Qt include(s)
#include <QSplitter>

// UnderBudget include(s)
#include <ledger/model/Envelope.hpp>

// Forward declaration(s)
class QWidget;

namespace ub {
namespace ledger {

// Forward declaration(s)
class EnvelopeDetailsDialog;
class EnvelopeModel;
class EnvelopeTransactionModel;
class Transaction;
class TransactionTableView;
class TreeView;

/**
 * Widget for displaying a list of envelopes and associated transactions.
 *
 * @ingroup ledger
 */
class EnvelopeListWidget : public QSplitter {
    Q_OBJECT

public:
    /**
     * Initializes the envelope list widget.
     *
     * @param[in] model        Envelope model
     * @param[in] transactions Envelope transaction model
     * @param[in] parent       Parent widget
     */
    EnvelopeListWidget(EnvelopeModel * model, EnvelopeTransactionModel * transactions,
                       QWidget * parent);

signals:
    /**
     * Emitted to indicate the currently selected envelope.
     *
     * @param envelope Currently selected envelope
     */
    void envelopeSelected(const Envelope & envelope);

    /**
     * Emitted to indicate a request to modify a transaction.
     *
     * @param transaction Transaction to be modified
     */
    void modifyTransaction(const Transaction & transaction);

    /**
     * Emitted to indicate a request to duplicate a transaction.
     *
     * @param transaction Transaction to be duplicated
     */
    void duplicateTransaction(const Transaction & transaction);

    /**
     * Emitted to indicate a request to delete a transaction.
     *
     * @param transaction Transaction to be deleted
     */
    void deleteTransaction(const Transaction & transaction);

private slots:
    /**
     * Prompts the user to confirm deletion of the requested envelope.
     *
     * @param index Model index of the envelope to be deleted
     */
    void deleteEnvelope(const QModelIndex & index);

    /**
     * Updates the envelope transactions model to display transactions for the selected envelope.
     *
     * @param[in] current  Model index of the currently selected envelope
     * @param[in] previous Model index of the previoiusly selected envelope
     */
    void setTransactionFilter(const QModelIndex & current, const QModelIndex & previous);

    /**
     * Determines which transaction is selected and fires the @c modifyTransaction signal.
     *
     * @param index Model index of the envelope transaction to be modified
     */
    void modifyEnvelopeTransaction(const QModelIndex & index);

    /**
     * Determines which transaction is selected and fires the @c duplicateTransaction signal.
     *
     * @param index Model index of the envelope transaction to be duplicated
     */
    void duplicateEnvelopeTransaction(const QModelIndex & index);

    /**
     * Determines which transaction is selected and fires the @c deleteTransaction signal.
     *
     * @param index Model index of the envelope transaction to be deleted
     */
    void deleteEnvelopeTransaction(const QModelIndex & index);

private:
    /** Envelope model */
    EnvelopeModel * model_;
    /** Envelope transaction model */
    EnvelopeTransactionModel * transactions_;

    /** Envelope details dialog */
    EnvelopeDetailsDialog * details_;
    /** Envelope tree view */
    TreeView * tree_;
    /** Envelope transactions list view */
    TransactionTableView * transaction_list_;
};

} // ledger namespace
} // ub namespace
