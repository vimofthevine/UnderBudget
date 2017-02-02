// Qt include(s)
#include <QtWidgets>

// UnderBudget include(s)
#include "AccountListWidget.hpp"
#include "AccountModel.hpp"

namespace ub {
namespace ledger {

//--------------------------------------------------------------------------------------------------
AccountListWidget::AccountListWidget(AccountModel *model, QWidget *parent)
        : QTreeView(parent), model_(model) {
    setModel(model_);
    expandAll();

    // Give the name column the most weight
    header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    header()->setSectionResizeMode(0, QHeaderView::Stretch);

    // Setup context menu
    setContextMenuPolicy(Qt::DefaultContextMenu);

    setSelectionBehavior(QTreeView::SelectRows);
    setSelectionMode(QTreeView::SingleSelection);

    connect(this, &QTreeView::doubleClicked, this, &AccountListWidget::modifyAccount);
    connect(selectionModel(), &QItemSelectionModel::currentRowChanged,
            this, &AccountListWidget::selectAccount);
}

//--------------------------------------------------------------------------------------------------
void AccountListWidget::contextMenuEvent(QContextMenuEvent *event) {
    auto index = this->indexAt(event->pos());
    auto menu = new QMenu(this);

    if (index.isValid()) {
        auto add = new QAction(tr("Add child account"), this);
        connect(add, &QAction::triggered, this, [this] () {
            if (currentIndex().isValid()) {
                emit createAccount(currentIndex());
            }
        });

        auto del = new QAction(tr("Delete"), this);
        del->setEnabled(currentIndex().isValid());
        connect(add, &QAction::triggered, this, [this] () {
            if (currentIndex().isValid()) {
                emit deleteAccount(currentIndex());
            }
        });

        menu->addAction(add);
        menu->addAction(del);
    } else {
        auto add = new QAction(tr("Add top-level account"), this);
        connect(add, &QAction::triggered, this, [this] () {
            if (currentIndex().isValid()) {
                emit createAccount(QModelIndex());
            }
        });
        menu->addAction(add);
    }

    menu->exec(event->globalPos());
}

} // ledger namespace
} // ub namespace
