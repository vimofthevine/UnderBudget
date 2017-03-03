// Qt include(s)
#include <QtWidgets>

// UnderBudget include(s)
#include "TreeView.hpp"

namespace ub {
namespace ledger {

//--------------------------------------------------------------------------------------------------
TreeView::TreeView(QWidget *parent)
        : QTreeView(parent),
          filter_(new QSortFilterProxyModel(this)) {
    QTreeView::setModel(filter_);
    filter_->sort(0);

    // Setup context menu
    setContextMenuPolicy(Qt::DefaultContextMenu);

    setSelectionBehavior(QTreeView::SelectRows);
    setSelectionMode(QTreeView::SingleSelection);

    connect(this, &QTreeView::doubleClicked, this, [this] (const QModelIndex &index) {
                emit modifyItem(this->filter_->mapToSource(index));
            });

    this->installEventFilter(this);
}

//--------------------------------------------------------------------------------------------------
void TreeView::setModel(QAbstractItemModel *model) {
    filter_->setSourceModel(model);

    connect(model, &QAbstractItemModel::modelReset, this, [this] () {
        this->expandAll();
    });

    // Give the first column the most weight
    header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    header()->setSectionResizeMode(0, QHeaderView::Stretch);

    // Can't set this up until after the model has been set
    connect(selectionModel(), &QItemSelectionModel::currentRowChanged, this,
            [this] (const QModelIndex& current, const QModelIndex& previous) {
                emit selectItem(filter_->mapToSource(current), filter_->mapToSource(previous));
            });

    expandAll();
}

//--------------------------------------------------------------------------------------------------
void TreeView::contextMenuEvent(QContextMenuEvent *event) {
    auto index = this->indexAt(event->pos());
    auto menu = new QMenu(this);

    if (index.isValid()) {
        auto mod = new QAction(tr("Edit"), this);
        connect(mod, &QAction::triggered, this, [this] () {
            if (currentIndex().isValid()) {
                emit modifyItem(filter_->mapToSource(currentIndex()));
            }
        });

        auto add = new QAction(tr("Add child"), this);
        connect(add, &QAction::triggered, this, [this] () {
            if (currentIndex().isValid()) {
                emit createItem(filter_->mapToSource(currentIndex()));
            }
        });

        auto del = new QAction(tr("Delete"), this);
        connect(del, &QAction::triggered, this, [this] () {
            if (currentIndex().isValid()) {
                emit deleteItem(filter_->mapToSource(currentIndex()));
            }
        });

        menu->addAction(mod);
        menu->addAction(add);
        menu->addAction(del);
    } else {
        auto add = new QAction(tr("Add top-level item"), this);
        connect(add, &QAction::triggered, this, [this] () {
            if (currentIndex().isValid()) {
                emit createItem(QModelIndex());
            }
        });
        menu->addAction(add);
    }

    menu->exec(event->globalPos());
}

//--------------------------------------------------------------------------------------------------
bool TreeView::eventFilter(QObject *object, QEvent *event) {
    if (currentIndex().isValid()) {
        if (event->type() == QEvent::KeyPress) {
            QKeyEvent *key_event = static_cast<QKeyEvent *>(event);

            if ((key_event->key() == Qt::Key_Enter) or (key_event->key() == Qt::Key_Return)){
                emit modifyItem(filter_->mapToSource(currentIndex()));
            } else if (key_event->key() == Qt::Key_Delete) {
                emit deleteItem(filter_->mapToSource(currentIndex()));
            } else if (key_event->key() == Qt::Key_A) {
                emit createItem(filter_->mapToSource(currentIndex()));
            }
        }
    }

    return QTreeView::eventFilter(object, event);
}

} // ledger namespace
} // ub namespace