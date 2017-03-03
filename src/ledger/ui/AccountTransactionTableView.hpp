#pragma once

// Qt include(s)
#include <QTableView>

// Forward declaration(s)
class QSortFilterProxyModel;

namespace ub {
namespace ledger {

/**
 * Table view for displaying a list of account transactions.
 *
 * @ingroup ledger
 */
class AccountTransactionTableView : public QTableView {
    Q_OBJECT

public:
    /**
     * Initializes the account transaction table view.
     *
     * @param parent Parent widget
     */
    AccountTransactionTableView(QWidget * parent);

    void setModel(QAbstractItemModel * model) override;

private:
    /** Sort/filter proxy model */
    QSortFilterProxyModel * filter_;
};

} // ledger namespace
} // ub namespace
