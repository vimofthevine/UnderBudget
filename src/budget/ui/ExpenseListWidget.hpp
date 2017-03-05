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
class TreeView;
} // ledger namespace

namespace budget {
// Forward declaration(s)
class BudgetTableView;
class ExpenseModel;

/**
 * Widget for displaying a list of envelopes and associated budgeted expenses.
 *
 * @ingroup budget
 */
class ExpenseListWidget : public QSplitter {
    Q_OBJECT

public:
    /**
     * Initializes the expense list widget.
     *
     * @param[in] envelopes Envelope model
     * @param[in] expenses  Budgeted expenses model
     * @param[in] parent    Parent widget
     */
    ExpenseListWidget(ledger::EnvelopeModel * envelopes, ExpenseModel * expenses,
                      QWidget * parent);

signals:
    /**
     * Emitted to indicate the currently selected envelope.
     *
     * @param[in] current  Model index of the currently selected envelope
     * @param[in] previous Model index of the previoiusly selected envelope
     */
    void selectEnvelope(const QModelIndex & current, const QModelIndex & previous);

protected slots:
    /**
     * Prompts the user to confirm deletion of the requested envelope.
     *
     * @param index Model index of the envelope to be deleted
     */
    void deleteEnvelope(const QModelIndex & index);

    /**
     * Updates the expense model to display expenses for the selected envelope.
     *
     * @param[in] current  Model index of the currently selected envelope
     * @param[in] previous Model index of the previoiusly selected envelope
     */
    void setExpenseFilter(const QModelIndex & current, const QModelIndex & previous);

    /**
     * Displays the expense details dialog to allow modification of the requested expense.
     *
     * @param index Model index of the budgeted expense to be modified
     */
    void modifyExpense(const QModelIndex & index);

    /**
     * Displays the expense details dialog to allow duplication of the requested expense.
     *
     * @param index Model index of the budgeted expense to be duplicated
     */
    void duplicateExpense(const QModelIndex & index);

    /**
     * Prompts the user to confirm deletion of the requested budgeted expense.
     *
     * @param index Model index of the budgeted expense to be deleted
     */
    void deleteExpense(const QModelIndex & index);

private:
    /** Envelope model */
    ledger::EnvelopeModel * envelopes_;
    /** Budgeted expense model */
    ExpenseModel * expenses_;

    /** Envelope details dialog */
    ledger::EnvelopeDetailsDialog * details_;
    /** Envelope tree view */
    ledger::TreeView * tree_;
    /** Budgeted expense table view */
    BudgetTableView * table_;
};

} // budget namespace
} // ub namespace
