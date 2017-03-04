// Qt include(s)
#include <QtWidgets>

// UnderBudget include(s)
#include <ledger/model/Account.hpp>
#include <ledger/ui/AccountModel.hpp>
#include "AccountDetailsDialog.hpp"

namespace ub {
namespace ledger {

//--------------------------------------------------------------------------------------------------
AccountDetailsDialog::AccountDetailsDialog(AccountModel * model, QWidget * parent)
        : QDialog(parent), model_(model) {
    name_ = new QLineEdit(this);
    name_->setPlaceholderText(tr("Account name"));

    buttons_ = new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Reset |
                                    QDialogButtonBox::Cancel);
    connect(buttons_, &QDialogButtonBox::clicked, this, &AccountDetailsDialog::clicked);

    QFormLayout * form = new QFormLayout;
    form->addRow(tr("Name"), name_);
    form->addRow("", buttons_);

    setLayout(form);
}

//--------------------------------------------------------------------------------------------------
void AccountDetailsDialog::resetForNewAccount(const QModelIndex & parent) {
    parent_index_ = parent;
    account_index_ = QModelIndex();
    account_ = Account();
    name_->clear();
    show();
}

//--------------------------------------------------------------------------------------------------
void AccountDetailsDialog::showAccount(const QModelIndex & index) {
    parent_index_ = model_->parent(index);
    account_index_ = index;
    account_ = model_->account(index);
    name_->setText(account_.name());
    show();
}

//--------------------------------------------------------------------------------------------------
void AccountDetailsDialog::clicked(QAbstractButton * button) {
    if (button == buttons_->button(QDialogButtonBox::Save)) {
        Account account = account_;
        account.setName(name_->text());
        if (not account_index_.isValid()) {
            if (model_->create(account, parent_index_)) {
                emit accept();
            }
        } else {
            if (model_->update(account, account_index_)) {
                emit accept();
            }
        }
    } else if (button == buttons_->button(QDialogButtonBox::Reset)) {
        name_->setText(account_.name());
    } else if (button == buttons_->button(QDialogButtonBox::Cancel)) {
        emit reject();
    }
}

} // ledger namespace
} // ub namespace
