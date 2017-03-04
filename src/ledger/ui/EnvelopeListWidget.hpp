#pragma once

// Qt include(s)
#include <QSplitter>

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
    EnvelopeListWidget(EnvelopeModel *model, EnvelopeTransactionModel *transactions,
                      QWidget *parent);

signals:
    /**
     * Emitted to indicate the currently selected envelope.
     *
     * @param[in] current  Model index of the currently selected envelope
     * @param[in] previous Model index of the previoiusly selected envelope
     */
    void selectEnvelope(const QModelIndex &current, const QModelIndex &previous);

    /**
     * Emitted to indicate a request to modify a transaction.
     *
     * @param transaction Transaction to be modified
     */
    void modifyTransaction(const Transaction & transaction);

protected slots:
    /**
     * Prompts the user to confirm deletion of the requested envelope.
     *
     * @param index Model index of the envelope to be deleted
     */
    void deleteEnvelope(const QModelIndex &index);

    /**
     * Updates the envelope transactions model to display transactions for the selected envelope.
     *
     * @param[in] current  Model index of the currently selected envelope
     * @param[in] previous Model index of the previoiusly selected envelope
     */
    void setTransactionFilter(const QModelIndex &current, const QModelIndex &previous);

    /**
     * Determines which transaction is selected and fires the @c modifyTransaction signal.
     *
     * @param index Model index of the envelope transaction to be modified
     */
    void modifyEnvelopeTransaction(const QModelIndex & index);

private:
    /** Envelope model */
    EnvelopeModel *model_;
    /** Envelope transaction model */
    EnvelopeTransactionModel *transactions_;

    /** Envelope details dialog */
    EnvelopeDetailsDialog *details_;
    /** Envelope tree view */
    TreeView *tree_;
    /** Envelope transactions list view */
    TransactionTableView * transaction_list_;
};

} // ledger namespace
} // ub namespace
