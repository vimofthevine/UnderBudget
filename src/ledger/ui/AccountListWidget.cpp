// Qt include(s)
#include <QtWidgets>

// UnderBudget include(s)
#include "AccountDetailsDialog.hpp"
#include "AccountListWidget.hpp"
#include "AccountModel.hpp"
#include "AccountTreeView.hpp"

namespace ub {
namespace ledger {

//--------------------------------------------------------------------------------------------------
AccountListWidget::AccountListWidget(AccountModel *model, QWidget *parent)
        : QSplitter(Qt::Horizontal, parent), model_(model),
          details_(new AccountDetailsDialog(model_, parent)), tree_(new AccountTreeView(this)) {
    tree_->setModel(model_);

    details_->hide();
    details_->setModal(true);

    connect(tree_, &AccountTreeView::selectAccount, this, &AccountListWidget::selectAccount);
    connect(tree_, &AccountTreeView::createAccount,
            details_, &AccountDetailsDialog::resetForNewAccount);
    connect(tree_, &AccountTreeView::modifyAccount, details_, &AccountDetailsDialog::showAccount);
    connect(tree_, &AccountTreeView::deleteAccount, this, &AccountListWidget::deleteAccount);

    addWidget(tree_);
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

} // ledger namespace
} // ub namespace
