#pragma once

// Qt include(s)
#include <QTableView>

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
};

} // ledger namespace
} // ub namespace
