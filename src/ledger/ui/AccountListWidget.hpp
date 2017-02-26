#pragma once

// Qt include(s)
#include <QSplitter>

// Forward declaration(s)
class QWidget;

namespace ub {
namespace ledger {

// Forward declaration(s)
class AccountDetailsDialog;
class AccountModel;
class AccountTreeView;

/**
 * Widget for displaying a list of accounts and associated transactions.
 *
 * @ingroup ledger
 */
class AccountListWidget : public QSplitter {
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
     * Emitted to indicate the currently selected account.
     *
     * @param[in] current  Model index of the currently selected account
     * @param[in] previous Model index of the previoiusly selected account
     */
    void selectAccount(const QModelIndex &current, const QModelIndex &previous);

protected slots:
    /**
     * Prompts the user to confirm deletion of the requested account.
     *
     * @param index Model index of the account to be deleted
     */
    void deleteAccount(const QModelIndex &index);

private:
    /** Account model */
    AccountModel *model_;
    /** Account details dialog */
    AccountDetailsDialog *details_;
    /** Account tree view */
    AccountTreeView *tree_;
};

} // ledger namespace
} // ub namespace
