// Qt include(s)
#include <QtWidgets>

// UnderBudget include(s)
#include "AccountModel.hpp"
#include "AccountTreeView.hpp"

namespace ub {
namespace ledger {

//--------------------------------------------------------------------------------------------------
AccountTreeView::AccountTreeView(QWidget *parent)
        : QTreeView(parent),
          filter_(new QSortFilterProxyModel(this)) {
    QTreeView::setModel(filter_);
    filter_->sort(0);

    // Setup context menu
    setContextMenuPolicy(Qt::DefaultContextMenu);

    setSelectionBehavior(QTreeView::SelectRows);
    setSelectionMode(QTreeView::SingleSelection);

    connect(this, &QTreeView::doubleClicked, this, [this] (const QModelIndex &index) {
                emit modifyAccount(this->filter_->mapToSource(index));
            });

    this->installEventFilter(this);
}

//--------------------------------------------------------------------------------------------------
void AccountTreeView::setModel(QAbstractItemModel *model) {
    filter_->setSourceModel(model);

    connect(model, &QAbstractItemModel::modelReset, this, [this] () {
        this->expandAll();
    });

    // Give the name column the most weight
    header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    header()->setSectionResizeMode(AccountModel::NAME, QHeaderView::Stretch);

    // Can't set this up until after the model has been set
    connect(selectionModel(), &QItemSelectionModel::currentRowChanged, this,
            [this] (const QModelIndex& current, const QModelIndex& previous) {
                emit selectAccount(filter_->mapToSource(current), filter_->mapToSource(previous));
            });

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
                emit createAccount(filter_->mapToSource(currentIndex()));
            }
        });

        auto del = new QAction(tr("Delete"), this);
        del->setEnabled(currentIndex().isValid());
        connect(del, &QAction::triggered, this, [this] () {
            if (currentIndex().isValid()) {
                emit deleteAccount(filter_->mapToSource(currentIndex()));
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
                emit modifyAccount(filter_->mapToSource(currentIndex()));
            } else if (key_event->key() == Qt::Key_Delete) {
                emit deleteAccount(filter_->mapToSource(currentIndex()));
            } else if (key_event->key() == Qt::Key_A) {
                emit createAccount(filter_->mapToSource(currentIndex()));
            }
        }
    }

    return QTreeView::eventFilter(object, event);
}

} // ledger namespace
} // ub namespace
