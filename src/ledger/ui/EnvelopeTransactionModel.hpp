#pragma once

// Standard include(s)
#include <memory>
#include <vector>

// Qt include(s)
#include <QAbstractTableModel>
#include <QList>
#include <QModelIndex>
#include <QString>
#include <QVariant>

// UnderBudget include(s)
#include <ledger/model/Envelope.hpp>
#include <ledger/model/EnvelopeTransaction.hpp>

namespace ub {
namespace ledger {

// Forward declaration(s)
class LedgerRepository;
class TransactionRepository;

/**
 * Envelope transactions view model.
 *
 * @ingroup ledger
 */
class EnvelopeTransactionModel : public QAbstractTableModel {
    Q_OBJECT

public:
    /** Transaction date column */
    static const int DATE = 0;
    /** Transaction payee column */
    static const int PAYEE = 1;
    /** Transaction memo column */
    static const int MEMO = 2;
    /** Transaction amount column */
    static const int AMOUNT = 3;
    /** Envelope balance column */
    static const int BALANCE = 4;

    /**
     * Initializes the envelope transaction view model.
     */
    EnvelopeTransactionModel();

    /**
     * Updates the repositories used by the model.
     *
     * @param repository Ledger repository
     */
    void setRepository(std::shared_ptr<LedgerRepository> repository);

    int columnCount(const QModelIndex &parent) const override;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    int rowCount(const QModelIndex &parent) const override;

    QVariant data(const QModelIndex &index, int role) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

signals:
    /** Emitted when an error has occurred. */
    void error(const QString &message) const;

public slots:
    /**
     * Limits transactions presented by the model to those belonging to the given envelope.
     *
     * @param envelope Envelope for which to display transactions
     */
    void filterForEnvelope(const Envelope &envelope);

private:
    /** Transaction repository */
    std::shared_ptr<TransactionRepository> transactions_;
    /** Header titles */
    QList<QString> headers_;
    /** Current envelope */
    Envelope envelope_;
    /** Cached transactions */
    std::vector<EnvelopeTransaction> cache_;
};

} // ledger namespace
} // ub namespace
