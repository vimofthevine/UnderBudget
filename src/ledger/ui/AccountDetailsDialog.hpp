#pragma once

// Qt include(s)
#include <QDialog>
#include <QModelIndex>

// UnderBudget include(s)
#include <ledger/model/Account.hpp>

// Forward declaration(s)
class QAbstractButton;
class QDialogButtonBox;
class QLineEdit;
class QWidget;

namespace ub {
namespace ledger {

// Forward declaration(s)
class AccountModel;

/**
 * User input widget for creating and modifying accounts.
 *
 * @ingroup ledger
 */
class AccountDetailsDialog : public QDialog {
    Q_OBJECT

public:
    /**
     * Initializes the account details widget.
     *
     * @param model  Account model
     * @param parent Parent widget
     */
    AccountDetailsDialog(AccountModel *model, QWidget *parent);

public slots:
    /**
     * Updates the form to define a new account.
     *
     * @param[in] parent Model index of the parent account under which to create a new account
     */
    void resetForNewAccount(const QModelIndex &parent);

    /**
     * Updates the form to modify an existing account.
     *
     * @param[in] index Model index of the account to be modified
     */
    void showAccount(const QModelIndex &index);

protected slots:
    /**
     * Responds to a button click as appropriate.
     */
    void clicked(QAbstractButton *button);

private:
    /** Account model */
    AccountModel *model_;
    /** Account name input widget */
    QLineEdit *name_;
    /** Button box */
    QDialogButtonBox *buttons_;

    /** Parent model index */
    QModelIndex parent_index_;
    /** Account model index */
    QModelIndex account_index_;
    /** Account shown in the form */
    Account account_;
};

} // ledger namespace
} // ub namespace
