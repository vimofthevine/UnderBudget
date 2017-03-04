#pragma once

// Qt include(s)
#include <QTableView>

// Forward declaration(s)
class QSortFilterProxyModel;

namespace ub {
namespace ledger {

/**
 * Table view for displaying a list of transactions.
 *
 * @ingroup ledger
 */
class TransactionTableView : public QTableView {
    Q_OBJECT

public:
    /**
     * Initializes the transaction table view.
     *
     * @param parent Parent widget
     */
    TransactionTableView(QWidget * parent);

    void setModel(QAbstractItemModel * model) override;

signals:
    /**
     * Emitted to indicate a request to modify the specified item.
     *
     * @param index Model index of the item to be modified
     */
    void modifyItem(const QModelIndex & index);

    /**
     * Emitted to indicate a request to duplicate the specified item.
     *
     * @param index Model index of the item to be modified
     */
    void duplicateItem(const QModelIndex & index);

    /**
     * Emitted to indicate a request to delete the specified item.
     *
     * @param index Model index of the item to be deleted
     */
    void deleteItem(const QModelIndex & index);

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;

    bool eventFilter(QObject *object, QEvent *event) override;

private:
    /** Sort/filter proxy model */
    QSortFilterProxyModel * filter_;
};

} // ledger namespace
} // ub namespace
