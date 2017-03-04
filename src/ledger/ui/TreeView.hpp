#pragma once

// Qt include(s)
#include <QTreeView>

// Forward declaration(s)
class QAbstractItemModel;
class QContextMenuEvent;
class QSortFilterProxyModel;
class QWidget;

namespace ub {
namespace ledger {

/**
 * Tree view for displaying a hierarchical list of items.
 *
 * @ingroup ledger
 */
class TreeView : public QTreeView {
    Q_OBJECT

public:
    /**
     * Initializes the tree view.
     *
     * @param[in] parent Parent widget
     */
    TreeView(QWidget * parent);

    void setModel(QAbstractItemModel * model) override;

signals:
    /**
     * Emitted to indicate the currently selected item.
     *
     * @param[in] current  Model index of the currently selected item
     * @param[in] previous Model index of the previoiusly selected item
     */
    void selectItem(const QModelIndex & current, const QModelIndex & previous);

    /**
     * Emitted to indicate a request to create a new item under the specified parent.
     *
     * @param parent Model index of the parent item under which to create a new item
     */
    void createItem(const QModelIndex & parent);

    /**
     * Emitted to indicate a request to modify the specified item.
     *
     * @param index Model index of the item to be modified
     */
    void modifyItem(const QModelIndex & index);

    /**
     * Emitted to indicate a request to delete the specified item.
     *
     * @param index Model index of the item to be deleted
     */
    void deleteItem(const QModelIndex & index);

protected:
    void contextMenuEvent(QContextMenuEvent * event) override;

    bool eventFilter(QObject * object, QEvent * event) override;

private:
    /** Sort proxy model */
    QSortFilterProxyModel * filter_;
};

} // ledger namespace
} // ub namespace
