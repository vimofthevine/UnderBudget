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
#include <budget/model/Expense.hpp>
#include <budget/model/ExpenseRepository.hpp>
#include <ledger/model/Envelope.hpp>

namespace ub {
namespace budget {

// Forward declaration(s)
class BudgetRepository;
class ExpenseRepository;

/**
 * Budgeted expenses view model.
 *
 * @ingroup budget
 */
class ExpenseModel : public QAbstractTableModel {
    Q_OBJECT

public:
    /** Expense description column */
    static const int DESCRIPTION = 0;
    /** Expense beginning date column */
    static const int BEGINNING_DATE = 1;
    /** Expense ending date column */
    static const int ENDING_DATE = 2;
    /** Expense recurrence column */
    static const int RECURRENCE = 3;
    /** Expense amount column */
    static const int AMOUNT = 4;

    /**
     * Initializes the expense view model.
     */
    ExpenseModel();

    /**
     * Updates the repositories used by the model.
     *
     * @param repository Budget repository
     */
    void setRepository(std::shared_ptr<BudgetRepository> repository);

    /**
     * Returns the budgeted expense at the given expense model index.
     *
     * @param index Model index of the budgeted expense to be retrieved
     * @return Budgeted expense located at the given expense model index
     */
    Expense expense(const QModelIndex & index);

    int columnCount(const QModelIndex & parent) const override;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    int rowCount(const QModelIndex & parent) const override;

    QVariant data(const QModelIndex & index, int role) const override;

signals:
    /** Emitted when an error has occurred. */
    void error(const QString & message) const;

public slots:
    /**
     * Limits expenses presented by the model to those belonging to the given envelope.
     *
     * @param envelope Envelope for which to display budgeted expenses
     */
    void filterForEnvelope(const ledger::Envelope & envelope);

    /**
     * Resets the model with updated data from the repository.
     */
    void refresh();

private:
    /** Expense repository */
    std::shared_ptr<ExpenseRepository> expenses_;
    /** Header titles */
    QList<QString> headers_;
    /** Current envelope */
    ledger::Envelope envelope_;
    /** Cached expenses */
    std::vector<Expense> cache_;
};

} // budget namespace
} // ub namespace
