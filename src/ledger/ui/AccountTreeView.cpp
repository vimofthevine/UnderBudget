// Qt include(s)
#include <QtWidgets>

// UnderBudget include(s)
#include "AccountTreeView.hpp"

namespace ub {
namespace ledger {

//--------------------------------------------------------------------------------------------------
AccountTreeView::AccountTreeView(QWidget *parent) : QTreeView(parent) {
    // Setup context menu
    setContextMenuPolicy(Qt::DefaultContextMenu);

    setSelectionBehavior(QTreeView::SelectRows);
    setSelectionMode(QTreeView::SingleSelection);

    connect(this, &QTreeView::doubleClicked, this, &AccountTreeView::modifyAccount);

    this->installEventFilter(this);
}

//--------------------------------------------------------------------------------------------------
void AccountTreeView::setModel(QAbstractItemModel *model) {
    QTreeView::setModel(model);

    connect(model, &QAbstractItemModel::modelReset, this, [this] () {
        this->expandAll();
    });

    // Give the name column the most weight
    header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    header()->setSectionResizeMode(0, QHeaderView::Stretch);

    // Can't set this up until after the model has been set
    connect(selectionModel(), &QItemSelectionModel::currentRowChanged,
            this, &AccountTreeView::selectAccount);

    expandAll();
}

//--------------------------------------------------------------------------------------------------
void AccountTreeView::contextMenuEvent(QContextMenuEvent *event) {
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
        connect(del, &QAction::triggered, this, [this] () {
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

//--------------------------------------------------------------------------------------------------
bool AccountTreeView::eventFilter(QObject *object, QEvent *event) {
    if (currentIndex().isValid()) {
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent *key_event = static_cast<QKeyEvent *>(event);

            if ((key_event->key() == Qt::Key_Enter) or (key_event->key() == Qt::Key_Return)){
                emit modifyAccount(currentIndex());
            } else if (key_event->key() == Qt::Key_Delete) {
                emit deleteAccount(currentIndex());
            } else if (key_event->key() == Qt::Key_A) {
                emit createAccount(currentIndex());
            }
        }
    }

    return QTreeView::eventFilter(object, event);
}

} // ledger namespace
} // ub namespace
