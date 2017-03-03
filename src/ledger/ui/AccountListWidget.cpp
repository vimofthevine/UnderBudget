// Qt include(s)
#include <QtWidgets>

// UnderBudget include(s)
#include "AccountDetailsDialog.hpp"
#include "AccountListWidget.hpp"
#include "AccountModel.hpp"
#include "AccountTransactionModel.hpp"
#include "TransactionTableView.hpp"
#include "TreeView.hpp"

namespace ub {
namespace ledger {

//--------------------------------------------------------------------------------------------------
AccountListWidget::AccountListWidget(AccountModel *model, AccountTransactionModel *transactions,
        QWidget *parent)
        : QSplitter(Qt::Horizontal, parent), model_(model), transactions_(transactions),
          details_(new AccountDetailsDialog(model_, parent)), tree_(new TreeView(this)),
          transaction_list_(new TransactionTableView(this)) {
    tree_->setModel(model_);
    transaction_list_->setModel(transactions_);

    details_->hide();
    details_->setModal(true);

    connect(tree_, &TreeView::selectItem, this, &AccountListWidget::selectAccount);
    connect(tree_, &TreeView::selectItem, this, &AccountListWidget::setTransactionFilter);
    connect(tree_, &TreeView::createItem,
            details_, &AccountDetailsDialog::resetForNewAccount);
    connect(tree_, &TreeView::modifyItem, details_, &AccountDetailsDialog::showAccount);
    connect(tree_, &TreeView::deleteItem, this, &AccountListWidget::deleteAccount);

    addWidget(tree_);
    addWidget(transaction_list_);

    // Give transaction list stretch priority
    setStretchFactor(1, 1);
}

//--------------------------------------------------------------------------------------------------
void AccountListWidget::deleteAccount(const QModelIndex &index) {
    Account account = model_->account(index);
    auto answer = QMessageBox::question(this->parentWidget(), tr("Delete Account?"),
            tr("Are you sure you want to delete %0?").arg(account.name()));
    if (answer == QMessageBox::Yes) {
        model_->remove(index);
    }
}

//--------------------------------------------------------------------------------------------------
void AccountListWidget::setTransactionFilter(const QModelIndex &current,
                                             const QModelIndex &previous) {
    auto acct = model_->account(current);
    transactions_->filterForAccount(acct);
}

} // ledger namespace
} // ub namespace
