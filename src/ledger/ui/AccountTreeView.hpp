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
 * Tree view for displaying a hierarchical list of accounts.
 *
 * @ingroup ledger
 */
class AccountTreeView : public QTreeView {
    Q_OBJECT

public:
    /**
     * Initializes the account tree view.
     *
     * @param[in] parent Parent widget
     */
    AccountTreeView(QWidget *parent);

    void setModel(QAbstractItemModel *model) override;

signals:
    /**
     * Emitted to indicate the currently selected account.
     *
     * @param[in] current  Model index of the currently selected account
     * @param[in] previous Model index of the previoiusly selected account
     */
    void selectAccount(const QModelIndex &current, const QModelIndex &previous);

    /**
     * Emitted to indicate a request to create a new account under the specified parent.
     *
     * @param parent Model index of the parent account under which to create a new account
     */
    void createAccount(const QModelIndex &parent);

    /**
     * Emitted to indicate a request to modify the specified account.
     *
     * @param index Model index of the account to be modified
     */
    void modifyAccount(const QModelIndex &index);

    /**
     * Emitted to indicate a request to delete the specified account.
     *
     * @param index Model index of the account to be deleted
     */
    void deleteAccount(const QModelIndex &index);

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;

    bool eventFilter(QObject *object, QEvent *event) override;

private:
    /** Sort proxy model */
    QSortFilterProxyModel *filter_;
};

} // ledger namespace
} // ub namespace
