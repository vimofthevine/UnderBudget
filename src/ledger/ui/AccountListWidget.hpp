#pragma once

// Qt include(s)
#include <QTreeView>

// Forward declaration(s)
class QContextMenuEvent;
class QWidget;

namespace ub {
namespace ledger {

// Forward declaration(s)
class AccountModel;

/**
 * Widget for displaying a list of accounts.
 *
 * @ingroup ledger
 */
class AccountListWidget : public QTreeView {
    Q_OBJECT

public:
    /**
     * Initializes the account list widget.
     *
     * @param[in] model  Account model
     * @param[in] parent Parent widget
     */
    AccountListWidget(AccountModel *model, QWidget *parent);

signals:
    /**
     * Emitted to indicate a request to create a new account.
     *
     * @param[in] index Model index of the parent account under which to create a new account
     */
    void createAccount(const QModelIndex &index);

    /**
     * Emitted to indicate a request to delete an account.
     *
     * @param[in] index Model index of the account to be deleted
     */
    void deleteAccount(const QModelIndex &index);

    /**
     * Emitted to indicate a request to modify an account.
     *
     * @param[in] index Model index of the account to be modified
     */
    void modifyAccount(const QModelIndex &index);

    /**
     * Emitted to indicate the currently selected account.
     *
     * @param[in] current  Model index of the currently selected account
     * @param[in] previous Model index of the previoiusly selected account
     */
    void selectAccount(const QModelIndex &current, const QModelIndex &previous);

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;

private:
    /** Account model */
    AccountModel *model_;
};

} // ledger namespace
} // ub namespace
