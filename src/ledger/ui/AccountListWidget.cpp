// Qt include(s)
#include <QtWidgets>

// UnderBudget include(s)
#include "AccountDetailsDialog.hpp"
#include "AccountListWidget.hpp"
#include "AccountModel.hpp"

namespace ub {
namespace ledger {

//--------------------------------------------------------------------------------------------------
AccountListWidget::AccountListWidget(AccountModel *model, QWidget *parent)
        : QTreeView(parent), model_(model),
          details_(new AccountDetailsDialog(model_, parent)) {
    setModel(model_);
    expandAll();
    connect(model_, &AccountModel::modelReset, this, [this] () {
        this->expandAll();
    });

    // Give the name column the most weight
    header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    header()->setSectionResizeMode(0, QHeaderView::Stretch);

    // Setup context menu
    setContextMenuPolicy(Qt::DefaultContextMenu);

    setSelectionBehavior(QTreeView::SelectRows);
    setSelectionMode(QTreeView::SingleSelection);

    connect(selectionModel(), &QItemSelectionModel::currentRowChanged,
            this, &AccountListWidget::selectAccount);

    details_->hide();
    details_->setModal(true);
    connect(this, &QTreeView::doubleClicked,
            details_, &AccountDetailsDialog::showAccount);

    this->installEventFilter(this);
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
void AccountListWidget::contextMenuEvent(QContextMenuEvent *event) {
    auto index = this->indexAt(event->pos());
    auto menu = new QMenu(this);

    if (index.isValid()) {
        auto add = new QAction(tr("Add child account"), this);
        connect(add, &QAction::triggered, this, [this] () {
            if (currentIndex().isValid()) {
                details_->resetForNewAccount(currentIndex());
            }
        });

        auto del = new QAction(tr("Delete"), this);
        del->setEnabled(currentIndex().isValid());
        connect(del, &QAction::triggered, this, [this] () {
            if (currentIndex().isValid()) {
                deleteAccount(currentIndex());
            }
        });

        menu->addAction(add);
        menu->addAction(del);
    } else {
        auto add = new QAction(tr("Add top-level account"), this);
        connect(add, &QAction::triggered, this, [this] () {
            if (currentIndex().isValid()) {
                details_->resetForNewAccount(QModelIndex());
            }
        });
        menu->addAction(add);
    }

    menu->exec(event->globalPos());
}

//--------------------------------------------------------------------------------------------------
bool AccountListWidget::eventFilter(QObject *object, QEvent *event) {
    if (currentIndex().isValid()) {
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent *key_event = static_cast<QKeyEvent *>(event);

            if ((key_event->key() == Qt::Key_Enter) or (key_event->key() == Qt::Key_Return)){
                details_->showAccount(currentIndex());
            } else if (key_event->key() == Qt::Key_Delete) {
                deleteAccount(currentIndex());
            } else if (key_event->key() == Qt::Key_A) {
                details_->resetForNewAccount(currentIndex());
            }
        }
    }

    return QTreeView::eventFilter(object, event);
}

} // ledger namespace
} // ub namespace
