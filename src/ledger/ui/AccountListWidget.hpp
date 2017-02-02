#pragma once

// Qt include(s)
#include <QTreeView>

// Forward declaration(s)
class QWidget;

namespace ub {
namespace ledger {

// Forward declaration(s)
class AccountModel;

/**
 * Customized QTreeView for displaying a list of accounts.
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

private:
    /** Account model */
    AccountModel *model_;
};

} // ledger namespace
} // ub namespace
